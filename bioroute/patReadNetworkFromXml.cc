//----------------------------------------------------------------
// File: patReadNetworkFromXml.cc
// Author: Michel Bierlaire
// Creation: Thu Oct 30 14:02:15 2008
//----------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <xmlwrapp/xmlwrapp.h>
#include "patReadNetworkFromXml.h"
#include "patDisplay.h"
#include "patErrMiscError.h"

patReadNetworkFromXml::patReadNetworkFromXml(patString fName, 
					     patString nName, 
					     patBoolean ctn) : 
  fileName(fName), theNetwork(nName), cleanTheNetwork(ctn) {

  //  openstreetmap assigns a unique ID for a road with is two
  //  ways. The idea is to multiply this id by the following
  //  multiplier to obtain the id of the reverse link.

  multiplierForTwoWaysIds = 1000 ;
}


patBoolean patReadNetworkFromXml::readFile(patError*& err) {


  xml::init xmlinit ;  
  
  xml::tree_parser parser(fileName.c_str()); 
  xml::document &doc = parser.get_document(); 
  if (!parser) {
    stringstream str ;
    str << "Error while parsing " << fileName ;
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe());
    return patFALSE ;
  }


  xml::node theRoot = doc.get_root_node() ;

  xml::node::node_type theType = theRoot.get_type() ;

  xml::node::iterator rootIter(theRoot.begin()) ;
  xml::node::iterator rootEnd(theRoot.end()) ;

  map<patString,short> theNodeNames ;
  map<xml::node::node_type,short> theNodeTypes ;

  for (; rootIter != rootEnd ; ++rootIter) {
    patString nodeName(rootIter->get_name()) ;
    theNodeNames[nodeName]++ ;
    xml::node::node_type theType = rootIter->get_type() ;
    theNodeTypes[theType]++ ;

    if (nodeName == "bounds") {
      patReal minlat, maxlat, minlon, maxlon ;
      
      xml::attributes attr = rootIter->get_attributes() ;
      xml::attributes::iterator i; 
      for (i = attr.begin() ; i != attr.end() ; ++i) { 
	patString attrName(i->get_name()) ;
	if (attrName == "minlat") {
	  minlat = atof(i->get_value()) ;
	} 
	else if (attrName == "maxlat") {
	  maxlat = atof(i->get_value()) ;
	} 
	else if (attrName == "minlon") {
	  minlon = atof(i->get_value()) ;
	} 
	else if (attrName == "maxlon") {
	  maxlon = atof(i->get_value()) ;
	} 
	else {
	  stringstream str ;
	  str << "Unknown attribute of node '" 
	      << nodeName 
	      << "' in file " 
	      << fileName 
	      << ": '" 
	      << attrName 
	      << "'" ;
	  err = new patErrMiscError(str.str()) ;
	  WARNING(err->describe());
	  return patFALSE;
	}
      }
      theNetwork.setMapBounds(minlat, maxlat, minlon, maxlon) ;
    }
    else if (nodeName =="node") {
      patULong id ;
      patReal lat ;
      patReal lon ;
      patString name ;
      xml::attributes attr = rootIter->get_attributes() ;
      xml::attributes::iterator i; 
      for (i = attr.begin() ; i != attr.end() ; ++i) { 
	patString attrName(i->get_name()) ;
	if (attrName == "id") {
	  id = atoi(i->get_value()) ;
	  stringstream str ;
	  str << "Node " << id ;
	  name = patString(str.str()) ;
	} 
	else if (attrName == "lat") {
	  lat = atof(i->get_value()) ;
	} 
	else if (attrName == "lon") {
	  lon = atof(i->get_value()) ;
	} 
	else {
	  //	  WARNING("Ignore attribute: " << attrName) ;
	}
      }
      
      xml::node::iterator iter ;
      for (iter = rootIter->begin() ;
	   iter != rootIter->end() ;
	   ++iter) {
	if (patString(iter->get_name()) == "tag") {
	  xml::attributes attr = iter->get_attributes() ;
	  xml::attributes::iterator i; 
	  for (i = attr.begin() ; i != attr.end() ; ++i) { 
	    if (patString(i->get_value()) == "name") {
	      ++i ;
	      name = patString(i->get_value()) ;
	      break ;
	    }
	  }
	}	
      }
      
      patNode newNode(id,name,lat,lon) ;
      patBoolean success = theNetwork.addNode(newNode) ;
      if (!success) {
	stringstream str ;
	str << "Node " << newNode << " not added to the network" ;
	err = new patErrMiscError(str.str()) ;
	WARNING(err->describe()) ;
	return patFALSE;
      }
    }
    else if (nodeName == "relation") {
    }
    else if (nodeName == "text") {

    }
    else if (nodeName == "way") {
      patULong id ;
      patULong upNodeId ;
      patULong downNodeId ;
      patString theName ;
      xml::attributes attr = rootIter->get_attributes() ;
      xml::attributes::iterator i; 
      for (i = attr.begin() ; i != attr.end() ; ++i) { 
	patString attrName(i->get_name()) ;
	if (attrName == "id") {
	  id = atoi(i->get_value()) ;
	  stringstream str ;
	  str << "Link " << id ;
	  theName = patString(str.str()) ;
	  break ;
	} 
      }
      vector<patULong> nodes ;
      patBoolean oneWay(patFALSE) ;
      patString streetName ;
      xml::node::iterator linkIter ;
      map<patString,patReal> theAttributes ;

      for (linkIter = rootIter->begin() ; 
	   linkIter != rootIter->end() ; 
	   ++linkIter) {
	patString tagName(linkIter->get_name()) ;
	if (tagName == "nd") {
	  patULong nodeid = atoi(linkIter->get_attributes().begin()->get_value()) ;
	  nodes.push_back(nodeid) ;
	} 
	else if (tagName == "tag") {
	  xml::attributes attr = linkIter->get_attributes() ;
	  xml::attributes::iterator i; 
	  
	  for (i = attr.begin() ; i != attr.end() ; ++i) { 
	    
	    //	    DEBUG_MESSAGE("TAG = " << i->get_name() << "=" << i->get_value()) ;
	    if (patString(i->get_value()) == "name") {
	      ++i ;
	      theName = patString(i->get_value()) ;
	    }
	    else if (patString(i->get_value()) == "oneway") {
	      ++i ;
	      if ( (patString(i->get_value()) == "yes") ||  
		   (patString(i->get_value()) == "true") ) {
		oneWay = patTRUE ;
	      }
	    } 
	  }	  
	}
      }
      if (!nodes.empty()) {
	vector<patULong>::iterator j(nodes.begin()), k(nodes.begin()) ;
	++k ;
	patULong theArcId(id) ;
	for ( ;  k != nodes.end() ; ++j,++k) {
	  patBoolean success = theNetwork.addArcWithIds(theArcId,*j,*k,theName,map<patString,patReal>(),err) ;
	  if (err != NULL) {
	    WARNING(err->describe()) ;
	    return patFALSE ;
	  }
	  theNetwork.computeArcLength(theArcId,err) ;
	  if (err != NULL) {
	    WARNING(err->describe()) ;
	    return patFALSE ;
	  }
	  if (!oneWay) {
	    if (cleanTheNetwork) {
	      theArcId *= multiplierForTwoWaysIds ;
	      success = theNetwork.addArcWithIds(theArcId,*k,*j,theName,map<patString,patReal>(),err) ;
	      if (err != NULL) {
		WARNING(err->describe()) ;
		return patFALSE;
	      }
	      theNetwork.computeArcLength(theArcId,err) ;
	      if (err != NULL) {
		WARNING(err->describe()) ;
		return patFALSE  ;
	      }
	    }
	    else {
	      stringstream str ;
	      str << "The network cannot include two ways links (like link " << theArcId << "). You can do two things. Either change the network file to have only one ways, or add the statement <doNotCleanTheNetwork value=\"1\"/> at the beginning of the bioroute file. In this case, the reverse arc of two way links will be generated, and ids will be assigned." ;
	      err = new patErrMiscError(str.str()) ;
	      WARNING(err->describe()) ;
	      return patFALSE ;

	    }
	  }
	}
      }
    }
  }

  theNetwork.finalizeNetwork(cleanTheNetwork,err) ;

  if (err != NULL) {
    WARNING(err->describe());
    return patFALSE ;
  }

  return patTRUE ;
  
}


patNetwork patReadNetworkFromXml::getNetwork() {
  return theNetwork ;
}
