//----------------------------------------------------------------
// File: patLinkIterator.h
// Author: Michel Bierlaire
// Creation: Wed May 20 16:51:44 2009
//----------------------------------------------------------------

#ifndef patLinkIterator_h
#define patLinkIterator_h

#include "patIterator.h"
#include <list>

class patArc ;
class patNetwork ;

class patLinkIterator: public patIterator<patArc*> {
 public:
  patLinkIterator(vector<patULong>* lon, patNetwork* n) ;
  void first() ;
  void next() ;
  patBoolean isDone() ;
  patArc* currentItem() ;
 private:
  patNetwork* theNetwork ;
  vector<patULong>* listOfNodes ;
  vector<patULong>::iterator upNode ;
  vector<patULong>::iterator downNode ;
};

#endif


