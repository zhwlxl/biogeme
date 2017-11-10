//----------------------------------------------------------------
// File: patNodeIterator.h
// Author: Michel Bierlaire
// Creation: Mon Aug 16 10:34:24 2010
//----------------------------------------------------------------

#ifndef patNodeIterator_h
#define patNodeIterator_h

#include <map>
#include "patType.h"
#include "patIterator.h"

class patNode ;

class patNodeIterator: public patIterator<patNode*> {

 public:
  patNodeIterator(map<patULong, patNode>* aMap) ;
  void first()  ;
  void next()  ;
  patBoolean isDone()  ;
  patNode* currentItem()  ;

 private:
  map<patULong,patNode>* theMap ;
  map<patULong,patNode>::iterator theIter ;
};


#endif
