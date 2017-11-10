//----------------------------------------------------------------
// File: patNodeIterator.cc
// Author: Michel Bierlaire
// Creation: Mon Aug 16 10:35:40 2010
//----------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "patNode.h"
#include "patNodeIterator.h"
#include "patDisplay.h"

patNodeIterator::patNodeIterator(map<patULong,patNode>* aMap): 
  theMap(aMap) {

}

void patNodeIterator::first() {
  if (theMap != NULL) {
    theIter = theMap->begin() ;
  }
}
void patNodeIterator::next() {
  if (theMap != NULL) {
    ++theIter ;
  }  
}
patBoolean patNodeIterator::isDone() {
  if (theMap != NULL) {
    return (theIter == theMap->end()) ;
  }  
  else {
    return patTRUE ;
  }
}

patNode* patNodeIterator::currentItem() {
  if (theMap != NULL) {
    return &(theIter->second) ;
  }
  else {
    return NULL ;
  }
}

