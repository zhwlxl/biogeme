//----------------------------------------------------------------
// File: patArcIterator.h
// Author: Michel Bierlaire
// Creation: Mon Aug 16 10:34:24 2010
//----------------------------------------------------------------

#ifndef patArcIterator_h
#define patArcIterator_h

#include <map>
#include "patType.h"
#include "patIterator.h"

class patArc ;

class patArcIterator: public patIterator<patArc*> {

 public:
  patArcIterator(map<patULong, patArc>* aMap) ;
  void first()  ;
  void next()  ;
  patBoolean isDone()  ;
  patArc* currentItem()  ;

 private:
  map<patULong,patArc>* theMap ;
  map<patULong,patArc>::iterator theIter ;
};


#endif
