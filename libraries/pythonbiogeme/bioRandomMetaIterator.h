//-*-c++-*------------------------------------------------------------
//
// File name : bioRandomMetaIterator.h
// Author :    Michel Bierlaire 
// Date :      Mon Jul  3 07:55:30 2017
//
//--------------------------------------------------------------------

#ifndef bioRandomMetaIterator_h
#define bioRandomMetaIterator_h

#include "patError.h"
#include "bioIterator.h"
#include "bioIteratorSpan.h"
#include "patVariables.h"

class bioIteratorInfo ;

/*!  This class represents an iterator on another iterator. However,
  it iterate randomly on the items. This is desgined for bootstrapping
  panel data.
  The range of the iterator is defined by "aSpan".
  The "threadSpan" defines the range designed for a given thread.
  Rule: assume aSpan = [l,u] and threadSpan[b,e].
  Then every item i in the iterations such that i is strictly before b
  are ignored. Note that the upper bound e is completely ignored. This is on purpose, in order to avoid possible misalignent with the meta iterators.
*/


class bioRandomMetaIterator: public bioIterator<bioIteratorSpan> {

public:
  bioRandomMetaIterator(bioIteratorSpan aSpan, 
			patError*& err) ;
    
  void first() ;
  void next() ; 
  patBoolean isDone() ;
  bioIteratorSpan currentItem() ;
  patULong nbrOfItems() const ;

private:
  vector<patULong> rowPointers ;
  patString theIteratorName ;
  patULong firstRow ;
  patULong lastRow ;
  patULong selectedRow ;
  patULong counter ;
  vector<patULong> validRows ;

};

#endif
