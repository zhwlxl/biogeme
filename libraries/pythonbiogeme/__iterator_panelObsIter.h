#ifndef __iterator_panelObsIter_h
#define __iterator_panelObsIter_h

#include "bioIterator.h"
#include "patVariables.h"

class bioSample ;

class __iterator_panelObsIter : public bioIterator<patVariables> {
  __iterator_panelObsIter(patULong fRow, patULong index, bioSample* aSample) ;
  void first() ;
  void next() ;
  patBoolean isDone() ;
  patVariables currentItem() ;
protected:
  patULong firstRow ;

  // The variables at position index is supposed to be the same for
  // all elements spanned by the iterator. When it changes, the
  // iterator stops. If patBadId, the iterator spans all observations.
  patULong theIndex ;
  patReal theTestValue  ;
  patVariables* theCurrentRow ;
  bioSample* theSample ;
  patULong currentRowIndex ;
};
#endif
