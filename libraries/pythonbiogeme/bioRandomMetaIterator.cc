//-*-c++-*------------------------------------------------------------
//
// File name : bioRandomMetaIterator.cc
// Author :    Michel Bierlaire
// Date :      Mon Jul  3 07:57:14 2017
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "patDisplay.h"
#include "bioRandomMetaIterator.h"
#include "bioIteratorInfoRepository.h"
#include "patRandomInteger.h"
#include "patErrMiscError.h"
#include "patErrNullPointer.h"
#include "patMath.h"

bioRandomMetaIterator::bioRandomMetaIterator(bioIteratorSpan aSpan, 
					     patError*& err) :
  theIteratorName(aSpan.name),
  firstRow(aSpan.firstRow),
  lastRow(aSpan.lastRow),
  counter(0) {

  DEBUG_MESSAGE("bioRandomMetaIterator Ctor for " << aSpan) ;
  rowPointers = bioIteratorInfoRepository::the()->getRowPointers(theIteratorName,err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return ;
  }

  for (patULong i = 0 ; i < rowPointers.size() ; ++i) {
    if (rowPointers[i] != patBadId) {
      validRows.push_back(i) ;
    }
  }
  DEBUG_MESSAGE("There are " << validRows.size() << " blocks of data") ;

  DEBUG_MESSAGE("Create a random iterator from row " << firstRow << " to row " << lastRow) ;
  
}


void bioRandomMetaIterator::first() {
  counter = 0 ;
}


void bioRandomMetaIterator::next() {
  ++counter ;
}


patBoolean bioRandomMetaIterator::isDone() {
  return (counter >= nbrOfItems()) ;
}


bioIteratorSpan bioRandomMetaIterator::currentItem() {
  selectedRow = patRandomInteger::the()->drawNumber(0,validRows.size()) ;
  bioIteratorSpan result ;
  result.name = theIteratorName ;
  result.firstRow = validRows[selectedRow] ; 
  result.lastRow = rowPointers[validRows[selectedRow]] ;
  return result ;
}


patULong bioRandomMetaIterator::nbrOfItems() const {
  return rowPointers.size() ;
}
