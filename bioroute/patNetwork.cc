//----------------------------------------------------------------
// File: patNetwork.cc
// Author: Michel Bierlaire
// Creation: Thu Oct 30 11:06:00 2008
//----------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <fstream>
#include "patNetwork.h"
#include "patMath.h"
#include "patConst.h"
#include "patAngle.h"
#include "patDisplay.h"
#include "patErrMiscError.h"
#include "patErrNullPointer.h"
#include "patErrOutOfRange.h"
#include "patGeoCoordinates.h"
#include "patShortestPathAlgoDest.h"
#include "patPathGenerationAlgo.h"
#include "patRandomNumberGenerator.h"
#include "patSequenceIterator.h"
#include "patNodeIterator.h"
#include "patArcIterator.h"
#include "patPathIterator.h"
#include "patPathListIterator.h"
#include "patComputeAngle.h"
  
patNetwork::patNetwork(patString theName) : 
  name(theName), 
  adjacencyLists(NULL),
  thresholdForLeftTurns(bioPi/2.0),
  arrowSpan(0.006) {

}

void patNetwork::setMapBounds(patReal minLat, 
			      patReal maxLat, 
			      patReal minLon, 
			      patReal maxLon) {
  minLatitude = minLat ;
  maxLatitude = maxLat ;
  minLongitude = minLon ;
  maxLongitude = maxLon ;
}


patBoolean patNetwork::addNode(const patNode& theNode) {
  map<patULong, patNode>::iterator found = theNodes.find(theNode.userId) ;
  if (found != theNodes.end()) {
    // The node Id already exists
    return patFALSE ;
  } 
  else {
    theNodes.insert(pair<patULong, patNode>(theNode.userId,theNode)) ; ; 
    //    DEBUG_MESSAGE("Node '" << theNode.name << "' added") ;  
    return patTRUE ;
  }
}

patBoolean patNetwork::addArcWithIds(patULong theId, 
				     patULong aNodeId, 
				     patULong bNodeId, 
				     patString theName,
				     map<patString,patReal> attributes,
				     patError*& err) {
  //  DEBUG_MESSAGE("Add arc " << theId << " from " << aNodeId << " to " << bNodeId) ;
  patNode* aNode = getNodeFromUserId(aNodeId) ;
  if (aNode == NULL) {
    err = new patErrNullPointer("patNode") ;
    WARNING(err->describe()) ;
    return patFALSE ;
  }
  patNode* bNode = getNodeFromUserId(bNodeId) ;
  if (bNode == NULL) {
    err = new patErrNullPointer("patNode") ;
    WARNING(err->describe()) ;
    return patFALSE ;
  }
  patBoolean result = addArc(theId,aNode,bNode,theName,attributes,err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return patFALSE ;
  }
  return result ;
}

patBoolean patNetwork::addArc(patULong theId, 
			      patNode* aNode, 
			      patNode* bNode, 
			      patString theName,
			      map<patString,patReal> attributes,
			      patError*& err) {

  if (aNode == NULL || bNode == NULL) {
    return patFALSE ;
  }
  map<patULong, patArc>::iterator found = theArcs.find(theId) ;
  if (found == theArcs.end()) {
    aNode->addSuccessor(bNode->userId) ;
    bNode->addPredecessor(aNode->userId) ;
    addNode(*aNode) ;
    addNode(*bNode) ;
    patArc newArc(theId,aNode,bNode,theName,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return patFALSE ;
    }
    newArc.polyline.push_back(aNode->geoCoord) ;
    newArc.polyline.push_back(bNode->geoCoord) ;
    newArc.attributes = attributes ;
    theArcs.insert(pair<patULong, patArc>(theId,newArc)) ;
    pair<patULong, patULong> pairOfNodeIds(aNode->userId, bNode->userId) ;
    pair<pair<patULong, patULong>, patULong > elementToInsert(pairOfNodeIds,theId) ;
    listOfArcsPerPairsOfNodes.insert(elementToInsert) ;

    return patTRUE ;
  }
  else {
    return patFALSE ;
  }
}


patNode* patNetwork::getNodeFromUserId(patULong id) {
  map<patULong, patNode>::iterator found = theNodes.find(id) ;
  if (found == theNodes.end()) {
    return NULL ;
  }
  else {
    return &(found->second) ;
  }
}

patArc* patNetwork::getArcFromUserId(patULong id)  {
  map<patULong, patArc>::iterator found = theArcs.find(id) ;
  if (found == theArcs.end()) {
    return NULL ;
  }
  else {
    return &(found->second) ;
  }
}

patArc* patNetwork::getArcFromInternalId(patULong id) {
  if (id >= internalArcs.size()) {
    return NULL ;
  }
  return internalArcs[id] ;
}



patArc* patNetwork::getArcFromNodesUserId(patULong aNodeId, patULong bNodeId)  {

  pair<patULong, patULong> elementToFind(aNodeId,bNodeId) ;
  map<pair<patULong, patULong>, patULong >::const_iterator found =
    listOfArcsPerPairsOfNodes.find(elementToFind) ;
  if (found == listOfArcsPerPairsOfNodes.end()) {
    return NULL ;
  }
  else {
    return getArcFromUserId(found->second) ;
  }
}

short patNetwork::isArcOneWay(patULong aNodeId, patULong bNodeId) const {
  
  short result(0) ;
  pair<patULong, patULong> elementToFind(aNodeId,bNodeId) ;
  map<pair<patULong, patULong>, patULong >::const_iterator found =
    listOfArcsPerPairsOfNodes.find(elementToFind) ;
  if (found != listOfArcsPerPairsOfNodes.end()) {
    ++result ;
  }

  elementToFind = pair<patULong, patULong>(bNodeId,aNodeId) ;
  found = listOfArcsPerPairsOfNodes.find(elementToFind) ;
  if (found != listOfArcsPerPairsOfNodes.end()) {
    ++result ;
  }
  return result ;
  
}



void patNetwork::setArcLength(patULong arcId, patReal l, patError*& err) {
  patArc* theArc = getArcFromUserId(arcId) ;
  if (theArc == NULL) {
    stringstream str ;
    str << "Arc " << arcId << " not found" ;
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe()) ;
    return ;
  }
  theArc->setLength(l) ;
}

void patNetwork::computeArcLength(patULong arcId, patError*& err) {
  patArc* theArc = getArcFromUserId(arcId) ;
  if (theArc == NULL) {
    stringstream str ;
    str << "Arc " << arcId << " not found" ;
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe()) ;
    return ;
  }
  patNode* upNode = getNodeFromUserId(theArc->upNodeId) ;
  if (upNode == NULL) {
    stringstream str ;
    str << "Node " << theArc->upNodeId << " not found" ;
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe()) ;
    return ;
  }
  patNode* downNode = getNodeFromUserId(theArc->downNodeId) ;
  if (downNode == NULL) {
    stringstream str ;
    str << "Node " << theArc->downNodeId << " not found" ;
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe()) ;
    return ;
  }
  theArc->setLength(upNode->geoCoord.distanceTo(downNode->geoCoord)) ;
  return ;
}

patULong patNetwork::nbrOfArcs() const {
  return theArcs.size() ;
}

patULong patNetwork::nbrOfNodes() const {
  return theNodes.size() ;
}

void patNetwork::writeKML(patString fileName, patError*& err) {
  ofstream kml(fileName.c_str()) ;
  kml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl ;
  kml << "      <kml xmlns=\"http://earth.google.com/kml/2.0\">" << endl ;
  kml << "      <Document>" << endl ;
  patString theName(name) ;
  replaceAll(&theName,patString("&"),patString("and")) ;
  kml << "            <name>" << theName <<"</name>" << endl ;
  kml << "            <description>File created by bioroute (Michel Bierlaire, EPFL)</description>" << endl ;

  kml << "<Folder>" << endl ;
  kml << "<name>Nodes</name>" << endl ;
  for (map<patULong, patNode>::iterator nIter = theNodes.begin() ;
       nIter != theNodes.end() ;
       ++nIter) {
  
    kml << "            <Placemark>" << endl ;
    if (nIter->second.isCentroid) {
      kml << "	<visibility>1</visibility>" << endl ;
    }
    else {
      kml << "	<visibility>0</visibility>" << endl ;
    }
  patString theName(nIter->second.name) ;
  replaceAll(&theName,patString("&"),patString("and")) ;
    kml << "                  <name>" << theName << "</name>" << endl ;
    kml << "                  <description>Id "<< nIter->second.userId <<"</description>" << endl ;
    kml << "                  <Point>" << endl ;
    kml << "                        <coordinates>" 
	<< nIter->second.geoCoord.getKML() << ", 0</coordinates>" << endl ;
    kml << "                  </Point>" << endl ;
    kml << "            </Placemark>" << endl ;
  }
  
  kml << "</Folder>" << endl ;
  kml << "<Folder>" << endl ;
  kml << "<name>Links</name>" << endl ;
  
  for (map<patULong, patArc>::iterator aIter = theArcs.begin() ;
       aIter != theArcs.end() ;
       ++aIter) {
    patBoolean oneWay = isArcOneWay(aIter->second.upNodeId,aIter->second.downNodeId) ;
    

    kml << "            <Placemark>" << endl ;
    //    kml << "	<visibility>0</visibility>" << endl ;
    patString theName(aIter->second.name) ;
    replaceAll(&theName,patString("&"),patString("and")) ;
    kml << "                  <name>"<< theName <<"</name>" << endl ;
    kml << "                  <description>Arc "<< aIter->second.userId 
	<< " from node "
	<< aIter->second.upNodeId 
	<< " to node "
	<< aIter->second.downNodeId ;
    if (!aIter->second.attributes.empty()) {
      kml << "[" ;
      for (map<patString,patReal>::iterator i = aIter->second.attributes.begin() ;
	   i != aIter->second.attributes.end() ;
	   ++i) {
	if (i != aIter->second.attributes.begin()) {
	  kml << ", " ;
	}
	kml << i->first << "=" << i->second ;
      }
      kml << "]" ;
    }
    kml << "</description>" << endl ;
    kml << " " << endl ;
    kml << "                  <Style>" << endl ;
    kml << "                        <LineStyle>" << endl ;
    kml << "                              <color>ff00ff00</color>" << endl ;
    if (oneWay) {
      kml << "                              <width>2</width>" << endl ;
    }
    else {
      kml << "                              <width>4</width>" << endl ;
    }
    kml << "                        </LineStyle>" << endl ;
    kml << "                  </Style>" << endl ;
    kml << " " << endl ;
    kml << "                  <LineString>" << endl ;
    kml << "                        <coordinates>" << endl ;
    
    list<patGeoCoordinates>::iterator gIter(aIter->second.polyline.begin()) ;
    list<patGeoCoordinates>::iterator hIter(aIter->second.polyline.begin()) ;
    ++hIter ;
    for ( ; hIter != aIter->second.polyline.end() ; ++gIter, ++hIter) {
      
      patReal a1 = gIter->longitudeInDegrees ;
      patReal a2 = gIter->latitudeInDegrees ;
      patReal b1 = hIter->longitudeInDegrees ;
      patReal b2 = hIter->latitudeInDegrees ;
      
      // If the arc is one way, we draw an arrow

      // If the arc is two way, we need to shift it a little bit
      
      //     patReal x1 ;
      //     patReal x2 ;
      //     patReal y1 ;
      //     patReal y2 ;
      
      //     if (patAbs(b1-a1) <= 1.0e-6) {
      //       x1 = a1 + shift ;
      //       x2 = a2 ;
      //       y1 = b1 + shift ;
      //       y2 = b2 ;
      //     }
      //     else {
      //       patReal ratio = (b2-a2)/(b1-a1) ;
      //       x1 = a1 - shift * ratio ;
      //       x2 = a2 + shift ;
      
      //     }
      kml << a1 <<"," << a2 << ",0 "  
	  << b1 <<"," << b2 << ",0" << endl ;
    }
    kml << "</coordinates>	" << endl ;
    kml << "                  </LineString>" << endl ;
    
    // Draw the arrow
    kml << "            </Placemark>" << endl ;
    if (oneWay) {
      for (gIter = aIter->second.polyline.begin(), 
	     hIter = aIter->second.polyline.begin(),
	     ++hIter;
	   hIter != aIter->second.polyline.end() ; 
	   ++gIter, ++hIter) {
	
	patReal a1 = gIter->longitudeInDegrees ;
	patReal a2 = gIter->latitudeInDegrees ;
	patReal b1 = hIter->longitudeInDegrees ;
	patReal b2 = hIter->latitudeInDegrees ;
	
	patReal d1 = b1 - a1 ;
	patReal d2 = b2 - a2 ;
	patReal dperp1 = -d2 / sqrt(d1 * d1 + d2 * d2) ;
	patReal dperp2 = d1 / sqrt(d1 * d1 + d2 * d2);
	kml << "      <Placemark>" << endl ;
	kml << "      <LineString>" << endl ;
    kml << "<coordinates>	" << endl ;
	kml << a1 + 0.5 * d1 << "," << a2 + 0.5 * d2 << ",0" << endl ;
	kml << a1 + 0.45 * d1 + arrowSpan * dperp1 << "," << a2 + 0.45 * d2 + arrowSpan * dperp2 << endl ; 
	kml << a1 + 0.45 * d1 - arrowSpan * dperp1 << "," << a2 + 0.45 * d2 - arrowSpan * dperp2 << endl ; 
	kml << a1 + 0.5 * d1 << "," << a2 + 0.5 * d2 << ",0" << endl ;
	kml << "</coordinates>	" << endl ;
	kml << "      </LineString>" << endl ;
	kml << "      </Placemark>" << endl ;
      }
    }
  }
  kml << "</Folder>" << endl ;
  kml << " " << endl ;
  kml << "<Folder>" << endl ;
  kml << "<name>Shortest paths for selected ODs</name>" << endl ;
  for (set<patOd>::iterator i = shortestPathsToBeDisplayed.begin() ;
       i != shortestPathsToBeDisplayed.end() ;
       ++i) {
    patBoolean success ;
    patPath thePath = getShortestPath(*i, &success, err) ;
    if (err != NULL) {
      WARNING(err->describe()); 
      return ;
    }
    if (!success) {
      stringstream str ;
      str << "OD " << *i << " not connected" ;
      err =  new patErrMiscError(str.str()) ;
      WARNING(err->describe());
      return ;
    }
    stringstream str ;
    str << "Shortest path for " << *i ;
    kml << getKMLCode(thePath,patString(str.str()),patString("ffffffff")) << endl ;
  }
  kml << "</Folder>" << endl ;

  kml << "<Folder>" << endl ;
  kml << "<name>User defined paths</name>" << endl ;
  for (map<patULong,patPath>::iterator p = userDefinedPaths.begin() ;
       p != userDefinedPaths.end() ;
       ++p) {
    kml << getKMLCode(p->second,p->second.name,patString("cc0000ff")) ;
  }
 
  kml << "</Folder>" << endl ;
  if (!generatedPaths.empty()) {
    kml << "<Folder>" << endl ;
    kml << "<name>Generated paths</name>" << endl ;
    for (map<patString,list<patPath> >::iterator p = generatedPaths.begin() ;
	 p != generatedPaths.end() ;
	 ++p) {
      kml << "<Folder>" << endl ;
      kml << "<name>Set " << p->first ;
      if (listOfAlgos[p->first] != NULL) {
	kml << " generated by algorithm " << listOfAlgos[p->first]->getName()  ;
      }
      kml << "</name>" << endl ;
      for (list<patPath>::iterator k = p->second.begin() ;
	   k != p->second.end() ;
	   ++k) {
	kml << getKMLCode(*k,(*k).name,patString("cc0000ff")) ;
      }
      kml << "</Folder>" << endl ;

    }
    kml << "</Folder>" << endl ;
  }

  kml << "      </Document>" << endl ;
  kml << "      </kml>" << endl ;


  kml.close() ;
  GENERAL_MESSAGE("File " << fileName << " has been created") ;
}


void patNetwork::removeUselessNodes(patError*& err) {


  DEBUG_MESSAGE(theNodes.size() << " nodes") ;
  patBoolean erase(patTRUE) ;

  while (erase) {
    erase = patFALSE ;
    for (map<patULong, patNode>::iterator i = theNodes.begin() ;
	 i != theNodes.end() ;
	 ++i) {
      if (i->second.disconnected()) {
	theNodes.erase(i->first) ;
	erase = patTRUE ;
	break ;
      }
    }
  }

  DEBUG_MESSAGE("After removing disconnected nodes: " << theNodes.size() << " nodes") ;

  erase = patTRUE ;

  while (erase) {
    for (map<patULong, patNode>::iterator i = theNodes.begin() ;
	 i != theNodes.end() ;
	 ++i) {
      erase = removeUselessNode(i->first,err) ;
      if (err != NULL) {
	WARNING(err->describe()) ;
	return ;
      }
      if (erase) {
	break ;
      }
    }
  }

  DEBUG_MESSAGE("After removing intermediary nodes: " << theNodes.size() << " nodes") ;

}
patBoolean patNetwork::removeUselessNode(patULong id, patError*& err) {
  patNode* theNode = getNodeFromUserId(id) ;
  if (theNode == NULL) {
    return patFALSE ;
  }

  if (theNode->userPredecessors.size() != 1) {
    return patFALSE ;
  }
  patULong thePred = *(theNode->userPredecessors.begin()) ;
  patNode* thePredPtr = getNodeFromUserId(thePred) ;
  if (thePredPtr == NULL) {
    err = new patErrNullPointer("patNode") ;
    WARNING(err->describe()) ;
    return patFALSE ;
  }
  if (theNode->userSuccessors.size() != 1) {
    return patFALSE ;
  }
  patULong theSucc = *(theNode->userSuccessors.begin()) ;

  patNode* theSuccPtr = getNodeFromUserId(theSucc) ;
  if (theSuccPtr == NULL) {
    err = new patErrNullPointer("patNode") ;
    WARNING(err->describe()) ;
    return patFALSE ;
  }
  theSuccPtr->userPredecessors.erase(id) ;
  theSuccPtr->userPredecessors.insert(thePred) ;
  thePredPtr->userSuccessors.erase(id) ;
  thePredPtr->userSuccessors.insert(theSucc) ; 

  patArc* arc1 = getArcFromNodesUserId(thePred,theNode->userId) ;
  if (arc1 == NULL) {
    WARNING("arc " << thePred << "->" << theNode->userId) ;
    err = new patErrNullPointer("patArc") ;
    WARNING(err->describe()) ;
    return patFALSE ;
  }
  patArc* arc2 = getArcFromNodesUserId(theNode->userId,theSucc) ;
  if (arc2 == NULL) {
    WARNING("arc " << theNode->userId << "->" << theSucc) ;
    err = new patErrNullPointer("patArc") ;
    WARNING(err->describe()) ;
    return patFALSE ;
  }

  patULong newId = patMin(arc1->userId,arc2->userId) ;
  patString newName ;
  if (arc1->name != arc2->name) {
    newName = arc1->name + " - " + arc2->name ;
  }
  else {
    newName = arc1->name ;
  }
  patArc newArc(newId,thePredPtr,theSuccPtr,newName,err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return patFALSE ;
  }
  newArc.generalizedCost = arc1->generalizedCost + arc2->generalizedCost ;
  newArc.length = arc1->length + arc2->length ;
  newArc.polyline.push_back(thePredPtr->geoCoord) ;
  newArc.polyline.push_back(theNode->geoCoord) ;
  newArc.polyline.push_back(theSuccPtr->geoCoord) ;

  theArcs.erase(arc1->userId) ;  
  theArcs.erase(arc2->userId) ;  
  theArcs.insert(pair<patULong, patArc>(newArc.userId,newArc)) ;

  pair<patULong, patULong> toremove1(thePred,id) ;
  listOfArcsPerPairsOfNodes.erase(toremove1) ;
  pair<patULong, patULong> toremove2(id,theSucc) ;
  listOfArcsPerPairsOfNodes.erase(toremove2) ;
  pair<patULong, patULong> toadd(thePred,theSucc) ;
  pair< pair<patULong, patULong>, patULong > toinsert(toadd,newArc.userId) ;
  listOfArcsPerPairsOfNodes.insert(toinsert) ;


  theNodes.erase(id) ;

  return patTRUE ;
		 
}

void patNetwork::addOd(patULong o, patULong d) {
  theOds.insert(patOd(o,d)) ;
}

void patNetwork::registerOds(patError*& err) {
  vector<patULong> unknownNodes ;
  for (set<patOd>::iterator i = theOds.begin() ;
       i != theOds.end() ;
       ++i) {
    patNode* orig = getNodeFromUserId(i->orig) ;
    if (orig == NULL) {
      unknownNodes.push_back(i->orig) ;
    }
    else {
      orig->isCentroid = patTRUE ;
    }
    patNode* dest = getNodeFromUserId(i->dest) ;
    if (dest == NULL) {
      unknownNodes.push_back(i->dest) ;
    }
    else {
      dest->isCentroid = patTRUE ;
    }
  }

  if (!unknownNodes.empty()) {
    stringstream str ;
    str << "The following centroids are unknown nodes: " ;
    for (vector<patULong>::iterator j = unknownNodes.begin() ;
	 j != unknownNodes.end() ;
	 ++j) {
      if (j != unknownNodes.begin()) {
	str << "-" ;
      }
      str << *j ;
    }
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe()) ;
    return ;
  }
}


patULong patNetwork::getInternalNodeIdFromUserId(patULong userId) const {
  map<patULong, patNode>::const_iterator found = theNodes.find(userId) ;
  if (found == theNodes.end()) {
    return patBadId ;
  }
  else {
    return found->second.internalId ;
  }
}


patULong patNetwork::getInternalArcIdFromUserId(patULong userId) const {
  map<patULong, patArc>::const_iterator found = theArcs.find(userId) ;
  if (found == theArcs.end()) {
    return patBadId ;
  }
  else {
    return found->second.internalId ;
  }

}

void patNetwork::computeInternalIds() {
  patULong arcInternalId(0) ;
  for (map<patULong, patArc>::iterator i = theArcs.begin() ;
       i != theArcs.end() ;
       ++i) {
    i->second.internalId = arcInternalId ;
    internalArcs.push_back(&(i->second)) ;
    ++arcInternalId ;
  }

  patULong nodeInternalId(0) ;
  for (map<patULong, patNode>::iterator i = theNodes.begin() ;
       i != theNodes.end() ;
       ++i) {
    i->second.internalId = nodeInternalId ;
    internalNodes.push_back(&(i->second)) ;
    ++nodeInternalId ;
  }
	
}

void patNetwork::finalizeNetwork(patBoolean cleanTheNetwork, patError*& err) {

  if (cleanTheNetwork) {
    DEBUG_MESSAGE("Clean the network") ;
    removeUselessNodes(err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return ;
    }
  }
  registerOds(err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return ;
  }
  computeInternalIds() ;

  buildAdjacencyLists(err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return ;
  }

  buildSubpaths(err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return ;
  }

  buildUserDefinedPaths(err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return ;
  }

  generatePaths(err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return ;
  }

  computeLinkAdditivePathAttributes(err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return ;
  }
  
  if (leftTurns) {
    computeLeftTurns(err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return ;
    }
  }
}


void patNetwork::buildAdjacencyLists(patError*& err) {
  if (adjacencyLists == NULL) {
    adjacencyLists = new vector< list <  pair<patReal,patULong> > > ;
  }
  else {
    adjacencyLists->erase(adjacencyLists->begin(),adjacencyLists->end()) ;
  }
  for (patULong n = 0 ; n < internalNodes.size() ; ++n) {
    list <  pair<patReal,patULong> >  currentList ;
    patNode* theNode = internalNodes[n] ;
    for (set<patULong>::iterator pred = theNode->userPredecessors.begin() ;
	 pred != theNode->userPredecessors.end() ;
	 ++pred) {
      patArc* theArc = getArcFromNodesUserId(*pred,theNode->userId) ;
      if (theArc == NULL) {
	stringstream str ;
	str << "Arc " << *pred << "->" << theNode->userId << " does not exist" ;
	err = new patErrMiscError(str.str()) ;
	WARNING(err->describe()) ;
	return ;
      }
      patULong internalIdPred = getInternalNodeIdFromUserId(*pred) ;
      currentList.push_back(pair<patReal,patULong>(theArc->generalizedCost,
						   internalIdPred)) ;
    }
    adjacencyLists->push_back(currentList) ;
  }
}

void patNetwork::computeMinimalLabel(patError*& err) {
  minimumLabelForShortestPath = patMaxReal ;
  for (map<patULong, patArc>::iterator i = theArcs.begin() ;
       i != theArcs.end() ;
       ++i) {
    if (i->second.generalizedCost < minimumLabelForShortestPath) {
      minimumLabelForShortestPath = i->second.generalizedCost ;
    }
  }
}

patReal patNetwork::getShortestPathCost(patOd od, patError*& err) {
  patBoolean success ;
  patPath theShortestPath = getShortestPath(od, &success, err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return patReal() ;
  }
  if (success) {
    patReal cost  = computePathCost(theShortestPath,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return patReal() ;
    }
    return cost ;
  }
  else {
    // OD not connected
    return patMaxReal ;
  }
}

patPath patNetwork::getShortestPath(patOd od, patBoolean* s, patError*& err) {
  // Should be optimized and called only when the generalized cost
  // have been modified.
  
  computeMinimalLabel(err) ;
  buildAdjacencyLists(err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return patPath();
  }
  
  patNode* origNode = getNodeFromUserId(od.orig) ; 
  if (origNode == NULL) {
    stringstream str ;
    str << "Node " << od.orig << " does not exist" ;
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe());
    return patPath() ;
  }
  patNode* destNode = getNodeFromUserId(od.dest) ; 
  if (destNode == NULL) {
    stringstream str ;
    str << "Node " << od.dest << " does not exist" ;
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe());
    return patPath() ;
  }

  patShortestPathAlgoDest theAlgo(destNode->internalId,
				  adjacencyLists,
				  minimumLabelForShortestPath) ;

  patBoolean success = theAlgo.computeShortestPathTree(err) ;

  if (err != NULL) {
    WARNING(err->describe()) ;
    return patPath() ;
  }

  if (!success) {
    err = new patErrMiscError("Negative cycle detected") ;
    WARNING(err->describe()) ;
    return patPath() ;
  }

  patShortestPathTreeDest theTree = theAlgo.getTree() ;

  patPath theShortestPath ;
  theShortestPath.od = od ;
  stringstream str ;
  str << "Shortest path from " << origNode->getName() << " to " << destNode->getName() ;
  theShortestPath.name = patString(str.str()) ;
  list<patULong> internalList = theTree.getShortestPathFrom(origNode->internalId,err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return patPath() ;
  }
  theShortestPath.listOfNodes = transformListOfInternalIntoExternalIds(internalList,err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return patPath() ;
  }

  if (!theShortestPath.checkOd()) {

    //    WARNING("OD of path " << theShortestPath << " does not macth the list of links") ;
    *s = patFALSE ;
    return patPath() ;
  }

  *s = patTRUE ;
  return theShortestPath ;

}
  


patString patNetwork::getKMLCode(patPath p,patString description, patString color) {
  patError* err(NULL) ;
  stringstream str ;
  str << "    <Placemark>" << '\n' ;
  str << "      <name>" << p.name << "</name>" << '\n' ;
  str << "      <description> " 
      << description 
      << " (Length=" 
      << computePathCost(p,err)  ;
  for (map<patString,patReal>::iterator i = p.attributes.begin() ;
       i != p.attributes.end() ;
       ++i) {
    str << ", " << i->first << "=" << i->second ;
  }
  str << ") " ;
  str << "</description>" << '\n' ;
  str << "                  <Style>" << endl ;
  str << "                        <LineStyle>" << endl ;
  str << "                              <color>"<< color <<"</color>" << endl ;
  str << "                              <width>5</width>" << endl ;
  str << "                        </LineStyle>" << endl ;
  str << "                  </Style>" << endl ;
  str << " " << endl ;
  str << "      <LineString>" << '\n' ;
  str << "        <visibility>0</visibility>" << '\n' ;
  str << "        <altitudeMode>absolute</altitudeMode>" << '\n' ;
  str << "        <coordinates>" << '\n' ;
  for (vector<patULong>::iterator i = p.listOfNodes.begin() ;
       i != p.listOfNodes.end() ;
       ++i) {
    str << getNodeFromUserId(*i)->geoCoord.longitudeInDegrees 
	<<"," 
	<< getNodeFromUserId(*i)->geoCoord.latitudeInDegrees 
	<< ",0 " << endl ;
  }
  str << "        </coordinates>" << '\n' ;
  str << "      </LineString>" << '\n' ;
  str << "    </Placemark>" << '\n' ;
  return patString(str.str()) ;
}


vector<patULong> patNetwork::transformListOfInternalIntoExternalIds(list<patULong> internalList, 
								  patError*& err) {
  vector<patULong> externalList ;

  for (list<patULong>::iterator i = internalList.begin() ;
       i != internalList.end() ;
       ++i) {
    if (*i >= internalNodes.size()) {
      err = new patErrOutOfRange<patULong>(*i,0,internalNodes.size()-1) ;
      WARNING(err->describe()) ;
      return vector<patULong>() ;
    }
    externalList.push_back(internalNodes[*i]->userId) ;
  }

  return externalList ;
}

patReal patNetwork::computePathCost(patPath aPath,patError*& err) {
  patReal totalCost(0.0) ;
  vector<patULong>::iterator upNode = aPath.listOfNodes.begin() ;
  vector<patULong>::iterator downNode = aPath.listOfNodes.begin() ;
  ++downNode ;
  while (downNode != aPath.listOfNodes.end()) {
    patArc* theArc = getArcFromNodesUserId(*upNode,*downNode) ;
    if (theArc == NULL) {
      stringstream str ;
      str << "Could not find the arc from node " << *upNode << " to node " << *downNode;
      err = new patErrMiscError(str.str()) ;
      WARNING(err->describe()) ;
      return patReal() ;
    }
    totalCost += theArc->generalizedCost ;
    ++upNode ;
    ++downNode ;
  }
  return totalCost ;
}

void patNetwork::generatePaths(patError*& err) {
  
  for (map<patString,patPathGenerationAlgo*>::iterator i = 
	 listOfAlgos.begin() ;
       i != listOfAlgos.end() ;
       ++i) {
    patPathGenerationAlgo* theGenerationAlgorithm = i->second ;
    list<patPath> theList ;
    for (set<patOd>::iterator od = theOds.begin() ;
	 od != theOds.end() ;
	 ++od) {
      theGenerationAlgorithm->generateAllPaths(this,*od,err) ;
      if (err != NULL) {
	WARNING(err->describe()) ;
	return ;
      }
      
      for (map<patPath,pair<patReal,patULong> >::iterator p = theGenerationAlgorithm->listOfPaths.begin() ;
	   p != theGenerationAlgorithm->listOfPaths.end() ;
	   ++p) {
	patPath thePath = p->first ;
	thePath.addAttribute(patString("pgLogProba"),p->second.first,err) ;
	if (err != NULL) {
	  WARNING(err->describe()) ;
	  return ;
	}
	thePath.addAttribute(patString("pgFrequency"),p->second.second,err) ;
	if (err != NULL) {
	  WARNING(err->describe()) ;
	  return ;
	}
	theList.push_back(thePath) ;
      }
    }
    generatedPaths[i->first] = theList ;
  }
}

void patNetwork::restoreOriginalCosts(patError*& err) {
  for (map<patULong, patArc>::iterator arc = theArcs.begin() ;
       arc != theArcs.end() ;
       ++arc) {
    arc->second.generalizedCost = arc->second.frozenGeneralizedCost ;
  }
  return ;
}

void patNetwork::generateMonteCarloCosts(patRandomNumberGenerator* aRndGen,
					 patReal stdErr,
					 patBoolean enforcePositivity,
					 patError*& err) {
  if (aRndGen == NULL) {
    err = new patErrNullPointer("patRandomNumberGenerator") ;
    WARNING(err->describe()) ;
    return ;
  }
  for (vector<patArc*>::iterator arc = internalArcs.begin() ;
       arc != internalArcs.end() ;
       ++arc) {
    (*arc)->frozenGeneralizedCost = (*arc)->generalizedCost ;
    patReal draw = aRndGen->getNextValue(err) ;
    if (err != NULL) {
      WARNING(err->describe());
      return ;
    }
    patReal perturb = (*arc)->generalizedCost * stdErr * draw ;
    (*arc)->generalizedCost += perturb ;
    if (enforcePositivity) {
      while ((*arc)->generalizedCost < 0.0) {
	patReal draw = aRndGen->getNextValue(err) ;
	if (err != NULL) {
	  WARNING(err->describe());
	  return ;
	}
	(*arc)->generalizedCost = 
	  (*arc)->frozenGeneralizedCost + 
	  (*arc)->frozenGeneralizedCost * stdErr * draw ;
      }
    }
  }
  return ;
}


patReal patNetwork::computeAngleNodes(patULong n1, patULong n2, patULong n3) {
    patGeoCoordinates p1 = getNodeFromUserId(n1)->geoCoord ;
    patGeoCoordinates p2 = getNodeFromUserId(n2)->geoCoord ;
    patGeoCoordinates p3 = getNodeFromUserId(n3)->geoCoord ;

    patComputeAngle ca(p1,p2,p3) ;
    return ca.getAngle() ;
}

int patNetwork::computeLeftTurns(patPath* aPath, patError*& err) {
  if (aPath == NULL) {
    err = new patErrNullPointer("patPath") ;
    WARNING(err->describe()) ;
    return 0 ;
  }
  if (aPath->listOfNodes.size() <= 2) {
    return 0 ;
  }
  vector<patULong>::iterator n1 = aPath->listOfNodes.begin() ;
  vector<patULong>::iterator n2 = n1 ;
  vector<patULong>::iterator n3 = n2 ;
  ++n2 ;
  ++n3 ;
  ++n3 ;

  int leftTurns(0) ;
  while (n3 != aPath->listOfNodes.end()) {
    patAngle angle(-computeAngleNodes(*n1,*n2,*n3)) ;
    if (!( angle < thresholdForLeftTurns)) {
      ++ leftTurns ;
    }
    ++n1 ;
    ++n2 ;
    ++n3 ;
  }
  return leftTurns ;
}

void patNetwork::computeLinkAdditivePathAttributes(patError*& err) {
  patIterator<patPath*>* theIter = getPathIterator() ;
  for (theIter->first() ;
       !theIter->isDone() ;
       theIter->next()) {
    patPath* aPath = theIter->currentItem() ;
    aPath->computeLinkAdditiveAttributes(this,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return ;
    }
  }
  DELETE_PTR(theIter);
}

void patNetwork::addSubpath(patULong id, list<patULong> subpathIds) {
  tempListOfSubpaths[id] = subpathIds ;
}

void patNetwork::addPath(patULong id, list<patULong> subpathIds) {
  tempListOfPaths[id] = subpathIds ;
}

void patNetwork::buildSubpaths(patError*& err) {
  for (map<patULong,list<patULong> >::iterator i = tempListOfSubpaths.begin() ;
       i != tempListOfSubpaths.end() ;
       ++i) {
    map<patULong,patSubpath*>::iterator found = allSubpaths.find(i->first) ;
    if (found == allSubpaths.end()) {
      createSubpath(i->first, err) ;
      if (err != NULL) {
	WARNING(err->describe()) ;
	return ;
      }
    }
  }
  list<patULong> listOfErrors ;
  for (map<patULong,patSubpath*>::iterator i = allSubpaths.begin() ;
       i != allSubpaths.end() ;
       ++i) {
    patBoolean check = i->second->checkConnectivity(this, err) ;
    if (!check) {
      listOfErrors.push_back(i->first) ;
    }
  }
  if (listOfErrors.size() > 0) {
    stringstream str ;
    str << "The following subpath(s) are not connected: " ;
    for (list<patULong>::iterator j = listOfErrors.begin() ;
	 j != listOfErrors.end() ;
	 ++j) {
      if (j != listOfErrors.begin()) {
	str << ", " ;
      }
      str << *j ;
    }
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe()) ;
    return ;
  }
}

patSubpath* patNetwork::createSubpath(patULong id, patError*& err) {
  //  DEBUG_MESSAGE("Create subpath " << id) ;
  map<patULong,patSubpath*>::iterator found = allSubpaths.find(id) ;
  if (found != allSubpaths.end()) {
    //DEBUG_MESSAGE("... already created") ;
    return found->second ;
  }
  map<patULong, patArc>::iterator arcFound = theArcs.find(id) ;
  if (arcFound != theArcs.end()) {
    patLinkSubpath theSp(&(arcFound->second)) ;
    theLinksSubpaths.push_back(theSp) ;
    allSubpaths[id] = &(theLinksSubpaths.back()) ;
    return allSubpaths[id] ;
  }

  map<patULong,list<patULong> >::iterator tmpfound = tempListOfSubpaths.find(id) ;
  if (tmpfound == tempListOfSubpaths.end()) {
    stringstream str ;
    str << "Subpath " << id << " not found" ;
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe()) ;
    return NULL ;
  }
  
  // Check first that all the subpaths in the list are created
  for (list<patULong>::iterator i = tmpfound->second.begin() ;
       i != tmpfound->second.end() ;
       ++i) {
    //    DEBUG_MESSAGE("Create subpath " << *i) ;
    createSubpath(*i,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return NULL ;
    }
  }
  
  patListSubpath theListSp(id) ;
  for (list<patULong>::iterator i = tmpfound->second.begin() ;
       i != tmpfound->second.end() ;
       ++i) {
    map<patULong,patSubpath*>::iterator spFound = allSubpaths.find(*i) ;
    if (spFound == allSubpaths.end()) {
      stringstream str ;
      str << "Subpath " << *i << " not in the database" ;
      err = new patErrMiscError(str.str()) ;
      WARNING(err->describe()) ;
      return NULL ;
    }
    theListSp.pushBackSubpath(spFound->second) ;
  }  
  theListSubpaths.push_back(theListSp) ;
  allSubpaths[id] = &(theListSubpaths.back()) ;
  return allSubpaths[id];
}

patString patNetwork::displaySubpaths(patError*& err) {
  stringstream str ;
  for (map<patULong,patSubpath*>::iterator i = allSubpaths.begin() ;
       i != allSubpaths.end() ;
       ++i) {
    str << i->second->getPath(err) << endl ;
  }
  return patString(str.str()) ;
}

patBoolean patNetwork::consecutiveArcs(patULong first, patULong second, patError*& err) {
  patArc* pFirst = getArcFromUserId(first) ;
  if (pFirst == NULL) {
    stringstream str ;
    str << "Unknown arc: " << first ;
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe());
    return patFALSE ;
  }
  patArc* pSecond = getArcFromUserId(second) ;
  if (pSecond == NULL) {
    stringstream str ;
    str << "Unknown arc: " << second ;
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe());
    return patFALSE ;
  }
  patBoolean result = pFirst->followedBy(pSecond,err) ;
  if (err != NULL) {
    WARNING(err->describe());
    return patFALSE ;
  }

   if (!result) {
     DEBUG_MESSAGE("NO. Not consecutive.") ;
     DEBUG_MESSAGE(*pFirst) ;
     DEBUG_MESSAGE(*pSecond) ;
   }
  return result ;
}

void patNetwork::addLinkAttributes(patULong id, 
				   pair<patString,patReal> attribute, 
				   patError*& err) {
  map<patULong, patArc>::iterator found = theArcs.find(id) ;
  if (found == theArcs.end()) {
    stringstream str ;
    str << "Arc " << id << " is unknown" ;
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe()) ;

    stringstream dbg ;
    for (found = theArcs.begin() ;
	 found != theArcs.end() ;
	 ++found) {
      if (found != theArcs.begin() ) {
	dbg << ", " ;
      }
      dbg << found->first ;
    }
    DEBUG_MESSAGE("List of " << theArcs.size() << " arcs: " << dbg.str()) ;
    return ;
  }
  found->second.attributes.insert(attribute) ;
}


patNetwork::patNetwork(const patNetwork& aNetwork) {
  copyObject(aNetwork) ;
}

patNetwork& patNetwork::operator=(const patNetwork& aNetwork) {
  theArcs.erase(theArcs.begin(),theArcs.end()) ;
  theNodes.erase(theNodes.begin(),theNodes.end()) ;
  internalArcs.erase(internalArcs.begin(),internalArcs.end()) ;
  internalNodes.erase(internalNodes.begin(),internalNodes.end()) ; 
  allSubpaths.erase(allSubpaths.begin(),allSubpaths.end()) ;
  copyObject(aNetwork) ;
  return *this ;
  
}


void patNetwork::copyObject(const patNetwork& aNetwork) {
  name = aNetwork.name ;
  arrowSpan = aNetwork.arrowSpan ;
  theArcs = aNetwork.theArcs ;
  theNodes = aNetwork.theNodes ;
  computeInternalIds() ;
  theOds = aNetwork.theOds ;
  listOfArcsPerPairsOfNodes = aNetwork.listOfArcsPerPairsOfNodes ;
  minLatitude = aNetwork.minLatitude ;
  maxLatitude = aNetwork.maxLatitude ;
  minLongitude = aNetwork.minLongitude ;
  maxLongitude = aNetwork.maxLongitude ;
  minimumLabelForShortestPath = aNetwork.minimumLabelForShortestPath ;
  patError* err(NULL) ;

  theListSubpaths = aNetwork.theListSubpaths ;
  theLinksSubpaths = aNetwork.theLinksSubpaths ;
  for (list<patLinkSubpath>::iterator i = theLinksSubpaths.begin() ;
       i != theLinksSubpaths.end() ;
       ++i) {
    allSubpaths[i->getId()] = &(*i) ;
  }
  shortestPathsToBeDisplayed = aNetwork.shortestPathsToBeDisplayed ;
  userDefinedPaths = aNetwork.userDefinedPaths ;
  thresholdForLeftTurns = aNetwork.thresholdForLeftTurns ;
  leftTurns = aNetwork.leftTurns ;
  listOfAlgos = aNetwork.listOfAlgos ;
  generatedPaths = aNetwork.generatedPaths ;
}
patString patNetwork::getName() const {
  return name ;
}


void patNetwork::addShortestPathToDisplay(patULong orig, patULong dest) {
  patOd theOd(orig,dest) ;
  shortestPathsToBeDisplayed.insert(theOd) ;
}

void patNetwork::buildUserDefinedPaths(patError*& err) {
  for (map<patULong,list<patULong> >::iterator p = tempListOfPaths.begin() ;
       p != tempListOfPaths.end() ;
       ++p) {
    patListSubpath theSubpath(patBadId) ;
    for (list<patULong>::iterator sp = p->second.begin() ;
	 sp != p->second.end() ;
	 ++sp) {
      map<patULong,patSubpath*>::iterator found = allSubpaths.find(*sp) ;
      if (found == allSubpaths.end()) {
	// Check if it is a link
	map<patULong, patArc>::iterator arcFound = theArcs.find(*sp) ;
	if (arcFound == theArcs.end()) {
	  stringstream str ;
	  str << "Subpath " << *sp << " unknown in the definition of path " << p->first ;
	  err = new patErrMiscError(str.str()) ;
	  WARNING(err->describe()) ;
	  return ;
	}
	else {
	  theSubpath.pushBackSubpath(createSubpath(*sp,err)) ;
	  if (err != NULL) {
	    WARNING(err->describe()) ;
	    return ;
	  }
	}
      }
      else {
	theSubpath.pushBackSubpath(found->second) ;
      }
    }
    userDefinedPaths[p->first] = theSubpath.getPath(err) ;
    stringstream str ;
    str << "User defined path " << p->first ;
    userDefinedPaths[p->first].name = patString(str.str()) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return ;
    }
  }
}

patPath* patNetwork::getPathFromUserId(patULong id) {
  map<patULong,patPath>::iterator found = userDefinedPaths.find(id) ;
  if (found == userDefinedPaths.end()) {
    return NULL ;
  }
  return &(found->second) ;
}


patIterator<patNode*>* patNetwork::getNodeIterator() {
  patIterator<patNode*>* result = new patNodeIterator(&theNodes) ;
}

patIterator<patArc*>* patNetwork::getArcIterator() {
  patIterator<patArc*>* result = new patArcIterator(&theArcs) ;
}

patIterator<patPath*>* patNetwork::getPathIterator() {
  
  patIterator<patPath*>* theUserIter = new patPathIterator(&userDefinedPaths) ;
  patSequenceIterator<patPath*>* allIterators = 
    new patSequenceIterator<patPath*>(theUserIter)  ;

  for (map<patString,list<patPath> >::iterator i = generatedPaths.begin() ;
       i != generatedPaths.end() ;
       ++i) {
    patPathListIterator* theIter = new patPathListIterator(&(i->second)) ;
    allIterators->addIterator(theIter) ;
  }
  return allIterators ;
}

void patNetwork::computeLeftTurns(patError*& err) {
  patIterator<patPath*>* theIter = getPathIterator() ;
  for (theIter->first() ;
       !theIter->isDone() ;
       theIter->next()) {
    patPath* aPath = theIter->currentItem() ;
    int leftTurns = computeLeftTurns(aPath,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return ;
    }
    aPath->addAttribute(patString("LeftTurns"),patReal(leftTurns),err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return ;
    }

  }
  DELETE_PTR(theIter);
}

void patNetwork::setThresholdForLeftTurns(patAngle threshold) {
  thresholdForLeftTurns = threshold ;
}

void patNetwork::mustComputeLeftTurns(patBoolean b) {
  leftTurns = b ;
}

void patNetwork::addPathGenerationAlgo(patString name, 
				       patPathGenerationAlgo* aAlgo, 
				       patError*& err) {
  map<patString,patPathGenerationAlgo*>::iterator found = listOfAlgos.find(name) ;
  if (found != listOfAlgos.end()) {
    stringstream str ;
    str << "Name " << name << " already used for a set of generated paths. Pls use a unique name for each set." ;
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe()) ;
    return ;
  }
  listOfAlgos[name] = aAlgo ;
}


vector<pair<patReal,patReal> > patNetwork::getDistanceFromAndToNode(patNode* node, patError*& err) {
  if (node == NULL) {
    err = new patErrNullPointer("patNode") ;
    WARNING(err->describe()); 
    return vector<pair<patReal,patReal> >() ;
  }
  vector<pair<patReal,patReal> > result(nbrOfNodes()) ;
  patULong nodeUserId = node->getUserId() ;
  for (patULong n = 0 ; n < internalNodes.size() ; ++n) {
    patOd od1(nodeUserId, internalNodes[n]->getUserId());
    patOd od2(internalNodes[n]->getUserId(),nodeUserId);
    patReal c1 = getShortestPathCost(od1,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return vector<pair<patReal,patReal> >() ;
    }
    patReal c2 = getShortestPathCost(od2,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return vector<pair<patReal,patReal> >() ;
    }
    result[internalNodes[n]->internalId] = pair<patReal,patReal>(c1,c2) ;
  }
  return result ;
  
}

patULong patNetwork::getUserNodeIdFromInternalId(patULong internalId) const {
  if (internalId >= internalNodes.size()) {
    return patBadId ;
  }
  return internalNodes[internalId]->getUserId() ;
}

patNode* patNetwork::getNodeFromInternalId(patULong internalId) {
  if (internalId >= internalNodes.size()) {
    return NULL ;
  }
  return internalNodes[internalId] ;

}

patBoolean patNetwork::isPathValid(patPath p) {
  for (patULong i = 0 ; i < p.listOfNodes.size()-1 ; ++i) {
    patArc* theArc = getArcFromNodesUserId(p.listOfNodes[i],
					   p.listOfNodes[i+1]) ;
    if (theArc == NULL) {
      DEBUG_MESSAGE("No arc between " << p.listOfNodes[i] << " and " << p.listOfNodes[i+1]) ;
      return patFALSE ;
    }
  }
  return patTRUE ;
}


void patNetwork::createGnuplotFile(patString gnuplotFileName, 
				   patString networkFileName, 
				   patString pathFileName,
				   patError*& err) {
  patAbsTime now ;
  now.setTimeOfDay() ;
  ofstream gnuplot(gnuplotFileName.c_str()) ;
  gnuplot << "# This file has been automatically generated" << endl ;
  gnuplot << "# " << now.getTimeString(patTsfFULL) << endl ;
  gnuplot << "" << endl ;
  gnuplot << "#" << endl ;
  gnuplot << "# SET THESE PARAMETERS" << endl ;
  gnuplot << "#" << endl ;
  gnuplot << "" << endl ;
  gnuplot << "plotscript = \"run.plt\"" << endl ;
  gnuplot << "network    = \"" << networkFileName << "\"" << endl ;
  gnuplot << "paths      = \"SET_THE_FILE_NAME_HERE.DAT\"" << endl ;

  gnuplot << "pathcount  = SET_THE_NUMBER_OF_PATHS_HERE # number of paths in the paths file" << endl ;
  gnuplot << "delay      = 1 # between consecutive paths in seconds" << endl ;
  gnuplot << "" << endl ;
  gnuplot << "#" << endl ;
  gnuplot << "# SET THE FIXED RANGE IN WHICH TO PLOT THE NETWORK" << endl ;
  gnuplot << "# " << endl ;
  gnuplot << "" << endl ;
  gnuplot << "reset" << endl ;
  gnuplot << "set xrange["<< minLongitude <<":" << maxLongitude <<"]" << endl ;
  gnuplot << "set yrange["<< minLatitude << ":" << maxLatitude  <<"]" << endl ;
  gnuplot << "" << endl ;
  gnuplot << "#" << endl ;
  gnuplot << "# START THE PLOTTING (in a separate file to enable a loop; a is the loop counter)" << endl ;
  gnuplot << "#" << endl ;
  gnuplot << "" << endl ;
  gnuplot << "unset border" << endl ;
  gnuplot << "unset xtics" << endl ;
  gnuplot << "unset ytics" << endl ;
  gnuplot << "unset key " << endl ;
  gnuplot << "a = -1" << endl ;
  gnuplot << "call plotscript network paths pathcount delay" << endl ;
  gnuplot << "" << endl ;
  gnuplot.close() ;

  GENERAL_MESSAGE("File " << gnuplotFileName << " created") ;


  now.setTimeOfDay() ;
  ofstream net(networkFileName.c_str()) ;
  net << "# This file has been automatically generated" << endl ;
  net << "# " << now.getTimeString(patTsfFULL) << endl ;
  net << "#" << endl ;
  net << "# THE NETWORK FILE" << endl ;
  net << "#" << endl ;
  net << "# every paragraph is one link from (x0,y0) to (x1,y1):" << endl ;
  net << "#" << endl ;
  net << "# x0	y0" << endl ;
  net << "# x1	y1" << endl ;
  net << "#" << endl ;
  net << "# leave one blank line between paragraphs for separated links" << endl ;

  for (map<patULong, patArc>::iterator aIter = theArcs.begin() ;
       aIter != theArcs.end() ;
       ++aIter) {

    list<patGeoCoordinates>::iterator firstPoint(aIter->second.polyline.begin()) ;
    list<patGeoCoordinates>::reverse_iterator lastPoint(aIter->second.polyline.rbegin()) ;
    patReal a1 = firstPoint->longitudeInDegrees ;
    patReal a2 = firstPoint->latitudeInDegrees ;
    patReal b1 = lastPoint->longitudeInDegrees ;
    patReal b2 = lastPoint->latitudeInDegrees ;
    
    net << endl ;
    net << a1 << '\t' << a2 << endl ;
    net << b1 << '\t' << b2 << endl ;
  }
  net.close() ;

  GENERAL_MESSAGE("File " << networkFileName << " created") ;
  patULong pathSetId = 0  ;
  for (map<patString,list<patPath> >::iterator setIter = generatedPaths.begin() ;
       setIter != generatedPaths.end() ;
       ++setIter) {
    ++pathSetId ;
    stringstream str ;
    str << pathSetId << "_" << pathFileName ;
    now.setTimeOfDay() ;
    patString aFileName(str.str()) ;
    ofstream path(aFileName.c_str()) ;
    path << "# This file has been automatically generated" << endl ;
    path << "# " << now.getTimeString(patTsfFULL) << endl ;
    path << "#" << endl ;
    path << "    " << endl ;
    path << "#" << endl ;
    path << "# THE PATHS FILE" << endl ;
    path << "# " << setIter->first << endl ;
    path << "# " << setIter->second.size() << " paths" << endl ;
    path << "#" << endl ;
    path << "# every paragraph is one path from (x0,y0) to (x1,y1) to (x2,y2) to ..." << endl ;
    path << "#" << endl ;
    path << "# x0	y0" << endl ;
    path << "# x1	y1" << endl ;
    path << "# x2	y2" << endl ;
    path << "# ..." << endl ;
    path << "#" << endl ;
    path << "# leave TWO blank lines between paragraphs to allow for path-by-path reading" << endl ;
    for (list<patPath>::iterator pIter = setIter->second.begin() ;
	 pIter != setIter->second.end() ;
	 ++pIter) {
      path << endl ;
      path << endl ;
      for (vector<patULong>::iterator nIter = pIter->listOfNodes.begin() ;
	   nIter != pIter->listOfNodes.end() ;
	   ++nIter) {
	patNode* theNode = getNodeFromUserId(*nIter) ;
	patReal x = theNode->geoCoord.longitudeInDegrees ;
	patReal y = theNode->geoCoord.latitudeInDegrees ;
	path << x << '\t' << y << endl ;
      }
    }
    path.close() ;
    GENERAL_MESSAGE("File " << aFileName << " created") ;
  }

}

void patNetwork::setArrowSpan(patReal as) {
  arrowSpan = as ;
}

void patNetwork::addPathFromMarkovProcess(patPath aPath) {
  patString m("Markov") ;
  generatedPaths[m].push_back(aPath) ;
}

void patNetwork::createPathStatistics(patString statFileName,patError*& err) {
  patAbsTime now ;
  now.setTimeOfDay() ;
  ofstream stat(statFileName.c_str()) ;
  stat << "# This file has been automatically generated" << endl ;
  stat << "# " << now.getTimeString(patTsfFULL) << endl ;
  stat << "" << endl ;
  
  for (map<patString,patPathGenerationAlgo*>::iterator setIter = listOfAlgos.begin() ;
       setIter != listOfAlgos.end();
       ++setIter) {
    if (setIter->second != NULL) {
      patPathGenerationAlgo* theAlgo = setIter->second ; 
      stat << "=================" << theAlgo->getName() << " ======================" << endl ;
      
      patULong pathId(0) ;
      for (map<patPath,pair<patReal,patULong> >::iterator pathIter = theAlgo->listOfPaths.begin() ;
	   pathIter != theAlgo->listOfPaths.end() ;
	   ++pathIter) {
	++pathId ;
	patULong count = pathIter->second.second ;
	patReal target = pathIter->first.getDeviation() ;
	stat << pathId << " --> " << pathIter->first << " [" << count << " times]["<< target << "][" << patReal(count)/target <<"]" << endl ;
      }
    }
  }
  stat.close() ;
  GENERAL_MESSAGE("File " << statFileName << " created") ;
}
