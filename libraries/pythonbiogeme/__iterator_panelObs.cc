#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "__iterator_panelObsIter.h"
#include "bioSample.h"
#include "patDisplay.h"

__iterator_panelObsIter::__iterator_panelObsIter(patULong fRow,
						 patULong index,
						 bioSample* aSample) :
  firstRow(fRow),
  theIndex(index),
  theSample(aSample) {

}
void __iterator_panelObsIter::first() {
  currentRowIndex = firstRow ;
  theCurrentRow = theSample->at(currentRowIndex) ;
  if (theCurrentRow ==NULL) return ;
  if (theIndex >= theCurrentRow->size()) {
    WARNING("INDEX OUT OF BOUNDS") ;
    return ;
  }
  theTestValue = (*theCurrentRow)[theIndex] ;
}

void __iterator_panelObsIter::next() {
  ++currentRowIndex ;
  theCurrentRow = theSample->at(currentRowIndex) ;
}
patBoolean __iterator_panelObsIter::isDone() {
  if (theCurrentRow == NULL) {
    return patTRUE ;
  }
  patReal theCurrentValue = (*theCurrentRow)[theIndex] ;
  return (theCurrentValue != theTestValue) ;
}

patVariables __iterator_panelObsIter::currentItem() {
  if (theCurrentRow != NULL) {
    return *theCurrentRow ;
  }
  else {
    WARNING("ITERATOR IS DONE") ;
    return patVariables() ;
  }
}

