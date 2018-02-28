//-*-c++-*------------------------------------------------------------
//
// File name : patIterator.h
// Author :    Michel Bierlaire
// Date :      Wed Dec  6 18:35:39 2017
//
//--------------------------------------------------------------------

#ifndef patIteratorErr_h
#define patIteratorErr_h

// Version of patIterator using err in the arguments
#include "patType.h"
#include "patConst.h"
#include "patError.h"

template <class T> class patIteratorErr {
public:
  virtual void first(patError*& err) = PURE_VIRTUAL ;
  virtual void next(patError*& err) = PURE_VIRTUAL ;
  virtual patBoolean isDone(patError*& err) = PURE_VIRTUAL ;
  virtual T currentItem(patError*& err) = PURE_VIRTUAL ;
  virtual ~patIteratorErr() {}
protected:
  patIteratorErr() {}
};

#endif
