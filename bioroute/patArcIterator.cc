//----------------------------------------------------------------
// File: patArcIterator.cc
// Author: Michel Bierlaire
// Creation: Sun Aug 22 17:55:31 2010
//----------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "patArc.h"
#include "patArcIterator.h"
#include "patDisplay.h"

patArcIterator::patArcIterator(map<patULong,patArc>* aMap): 
  theMap(aMap) {

}

void patArcIterator::first() {
  if (theMap != NULL) {
    theIter = theMap->begin() ;
  }
}
void patArcIterator::next() {
  if (theMap != NULL) {
    ++theIter ;
  }  
}
patBoolean patArcIterator::isDone() {
  if (theMap != NULL) {
    return (theIter == theMap->end()) ;
  }  
  else {
    return patTRUE ;
  }
}

patArc* patArcIterator::currentItem() {
  if (theMap != NULL) {
    return &(theIter->second) ;
  }
  else {
    return NULL ;
  }
}

