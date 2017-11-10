//----------------------------------------------------------------
// File: patPath.cc
// Author: Michel Bierlaire
// Creation: Mon Nov 17 22:27:29 2008
//----------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "patPath.h"
#include "patConst.h"
#include "patDisplay.h"
#include "patArc.h"
#include "patNode.h"
#include "patLinkIterator.h"
#include "patErrMiscError.h"
#include "patErrNullPointer.h"
#include "patErrOutOfRange.h"
#include "patNetwork.h"

patPath::patPath() : 
  name(patString("No name")) ,
  od(patBadId,patBadId),
  deviation(patMaxReal) {

}

patBoolean patPath::checkOd() {
  if (listOfNodes.empty()) {
    WARNING("Empty list of nodes") ;
    return patFALSE ;
  }
  patULong firstNode = *(listOfNodes.begin()) ;
  patULong lastNode = *(listOfNodes.rbegin()) ;
  patBoolean ok = patTRUE ;
  if (firstNode != od.orig) {
    WARNING("Origin of the path is " << firstNode << " and not " << od.orig) ;
    ok = patFALSE ;
  }
  if (lastNode != od.dest) {
    //    WARNING("Destination of the path is " << lastNode << " and not " << od.dest) ;
    ok = patFALSE ;
  }
  return ok ;
}

ostream& operator<<(ostream& str, const patPath& x) {
  str << x.name << " (from " << x.od.orig << " to " << x.od.dest << "): [" ;
  for (vector<patULong>::const_iterator i = x.listOfNodes.begin() ;
       i != x.listOfNodes.end() ;
       ++i) {
    if (i != x.listOfNodes.begin()) {
      str << "->" ;
    }
    str << *i ;
  }
  str << "]; " ;
  for (map<patString,patReal>::const_iterator i = x.attributes.begin() ;
       i != x.attributes.end() ;
       ++i) {
    if (i != x.attributes.begin()) {
      str << "; " ;
    }
    str << i->first << "=" << i->second ;
  }
  return str ;
}

patBoolean operator<(const patPath& p1, const patPath& p2) {
  if (p1.listOfNodes.size() < p2.listOfNodes.size()) {
    return patTRUE ;
  }
  if (p1.listOfNodes.size() > p2.listOfNodes.size()) {
    return patFALSE ;
  }
  vector<patULong>::const_iterator i1 = p1.listOfNodes.begin() ;
  vector<patULong>::const_iterator i2 = p2.listOfNodes.begin() ;
  while (i1 != p1.listOfNodes.end()) {
    if (*i1 < *i2) {
      return patTRUE ;
    }
    if (*i1 > *i2) {
      return patFALSE ;
    }
    ++i1 ;
    ++i2 ;
  }
  return patFALSE ;
}

patIterator<patArc*>* patPath::getLinkIterator(patNetwork* aNetwork) {
  patIterator<patArc*>* ptr = new patLinkIterator(&listOfNodes,aNetwork) ;
  return ptr ;
}


void patPath::computeLinkAdditiveAttributes(patNetwork* theNetwork, patError*& err) {
  attributes[patString("la_nbrLinks")] = 0.0 ;
  patIterator<patArc*>* theIterator = getLinkIterator(theNetwork) ;
  for (theIterator->first() ;
       !theIterator->isDone() ;
       theIterator->next()) {
    patArc* theArc = theIterator->currentItem() ;
    attributes[patString("la_nbrLinks")] += 1.0 ;
    for (map<patString,patReal>::const_iterator i = theArc->attributes.begin() ;
	 i != theArc->attributes.end() ;
	 ++i) {
      stringstream str ;
      str << "la_" << i->first ;
      attributes[patString(str.str())] += i->second ;
    }
  }
  
  DELETE_PTR(theIterator) ;
}

void patPath::addAttribute(patString name, patReal value, patError*& err) {
  map<patString,patReal>::iterator found = attributes.find(name) ;
  if (found != attributes.end()) {
    stringstream str ;
    str << "Attribute " << name << " is already defined for path " << *this;
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe()) ;
    return ;
  }
  attributes[name] = value ;
}

patPath patPath::extractSubPath(patULong uNodeIndex, 
				patULong dNodeIndex,
				patError*& err) {
  if (dNodeIndex < uNodeIndex) {
    stringstream str ;
    str << "The down node is in position " << dNodeIndex << " which is before the upnode (position " << uNodeIndex ;
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe()) ;
    return patPath() ;
  }
  if (dNodeIndex >= listOfNodes.size()) {
    err = new patErrOutOfRange<patULong>(dNodeIndex,0,listOfNodes.size()-1) ;
    WARNING(err->describe()) ;
    return patPath() ;
  }
  patPath result ;
  for (patULong i = uNodeIndex ;
       i <= dNodeIndex ;
       ++i) {
    result.listOfNodes.push_back(listOfNodes[i]) ;
  }

  stringstream str ;
  str << "Subpath of " << name ;
  result.name = patString(str.str()) ;
  result.od.orig = listOfNodes[uNodeIndex] ;
  result.od.dest = listOfNodes[dNodeIndex] ;
  return result ;
}


void patPath::pushFront(patPath p, patError*& err) {

  if (od.orig != p.od.dest) {
    stringstream str ;
    str << "Cannot connect paths " << p << " and " << *this ;
    err = new patErrMiscError(str.str());
    WARNING(err->describe()) ;
    return ;
  }
  
  if (p.listOfNodes.size() == 1) {
    return ;
  }

  listOfNodes.insert(listOfNodes.begin(),p.listOfNodes.begin(),p.listOfNodes.end()-1) ;
  stringstream n ;
  n << p.name << "+" << name ;
  name = patString(n.str()) ;
  od.orig = p.od.orig ;

  if (!checkOd()) {
    err = new patErrMiscError("Something went wrong during the merge. OD is not cinsistent.") ;
    WARNING(err->describe()) ;
    return ;
  }

}

void patPath::pushBack(patPath p, patError*& err) {

  if (od.dest != p.od.orig) {
    stringstream str ;
    str << "Cannot connect paths " << *this << " and " << p ;
    err = new patErrMiscError(str.str());
    WARNING(err->describe()) ;
    return ;
  }
  if (p.listOfNodes.size() == 1) {
    return ;
  }
  
  listOfNodes.insert(listOfNodes.end(),p.listOfNodes.begin()+1,p.listOfNodes.end()) ;
  stringstream n ;
  n << name << "+" << p.name ;
  name = patString(n.str()) ;
  od.dest = p.od.dest ;

  if (!checkOd()) {
    err = new patErrMiscError("Something went wrong during the merge. OD is not cinsistent.") ;
    WARNING(err->describe()) ;
    return ;
  }

}

void patPath::pushBack(patArc* a, patError*& err) {
  patULong  u = a->getUpNodeUserId()  ;
  patULong  d = a->getDownNodeUserId()  ;
  if (od.dest != u) {
    stringstream str ;
    str << "Cannot cannect path " << *this << " and link " << *a ;
    err = new patErrMiscError(str.str());
    WARNING(err->describe()) ;
    return ;
  }
  listOfNodes.push_back(d) ;
  stringstream n ;
  n << name << "+" << a->name ;
  name = patString(n.str()) ;
  od.dest = d ;

  if (!checkOd()) {
    err = new patErrMiscError("Something went wrong during the merge. OD is not cinsistent.") ;
    WARNING(err->describe()) ;
    return ;
  }
}


void patPath::overwrite(patPath p,
			patULong indexFirstNode,
			patULong indexLastNode,
			patError*& err) {

  if (listOfNodes[indexFirstNode] != p.od.orig) {
    stringstream str ;
    str << "Node at position " << indexFirstNode << " is " << listOfNodes[indexFirstNode] << " and does not coincide with the first node of path " << p ;
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe()) ;
    return ;
  }

  if (listOfNodes[indexLastNode] != p.od.dest) {
    stringstream str ;
    str << "Node at position " << indexLastNode << " is " << listOfNodes[indexLastNode] << " and does not coincide with the last node of path " << p ;
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe()) ;
    return ;
  }

  listOfNodes.erase(listOfNodes.begin()+indexFirstNode,listOfNodes.begin()+indexLastNode) ;

  listOfNodes.insert(listOfNodes.begin()+indexFirstNode,p.listOfNodes.begin(),p.listOfNodes.end()-1) ;
  stringstream str ;
  str << "Constructed path" ;
  name = patString(str.str()) ;

}


patReal patPath::getGeneralizedCost(patNetwork* aNetwork, patError*& err) {
  patReal result(0.0) ;
  patIterator<patArc*>* arcIter = getLinkIterator(aNetwork) ;  
  if (arcIter == NULL) {
    err = new patErrNullPointer("patIterator<patArc*>") ;
    WARNING(err->describe()) ;
    return patReal() ;
  }
  for (arcIter->first() ;
       !arcIter->isDone() ;
       arcIter->next()) {
    patArc* theArc = arcIter->currentItem() ;
    result += theArc->generalizedCost ;
  }
  return result ;
}

patULong patPath::getNodeIndex(patNode* aNode, 
			       patULong deb, 
			       patULong end, patError*& err) {
  if (end > listOfNodes.size()) {
    err = new patErrOutOfRange<patULong>(end,0,listOfNodes.size()) ;
    WARNING(err->describe()) ;
    return patBadId ;
  }
  for (patULong id = deb ; 
       id < end ;
       ++id) {
    if (listOfNodes[id] == aNode->getUserId()) {
      return id ;
    }
  }
  return patBadId ;
}

patBoolean patPath::contains(patULong nodeUserId) const {
  for (vector<patULong>::const_iterator i = listOfNodes.begin() ;
       i != listOfNodes.end() ;
       ++i) {
    if (*i == nodeUserId) {
      return patTRUE ;
    }
  }
  return patFALSE ;
}

patReal patPath::computeDeviationFromSpPath(patNetwork* theNetwork, 
					    patReal scale,
					    patError*& err) {
  
  if (theNetwork == NULL) {
    err = new patErrNullPointer("patNetwork") ;
    WARNING(err->describe()) ;
    return patReal()  ;
  }

  patReal shortestPathCost = theNetwork->getShortestPathCost(od,err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return patReal() ;
  }
  patReal pathCost = theNetwork->computePathCost(*this,err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return patReal() ;
  }
  deviation = (exp(scale * (shortestPathCost - pathCost))) ;
  return deviation ;
}

patReal patPath::getDeviation() const {
  return deviation ;
}
