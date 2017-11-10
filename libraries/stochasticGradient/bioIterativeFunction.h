//-*-c++-*------------------------------------------------------------
//
// File name : bioIterativeFunction.h
// Author :    Michel Bierlaire
// Date :      Mon Jul  3 19:51:58 2017
//
//--------------------------------------------------------------------

#ifndef bioIterativeFunction_h
#define bioIterativeFunction_h

#include "patType.h"
#include "patVariables.h"
#include "patError.h"

// Interface for the Stochastic Gradient algorithm. The function must
// be the sum of (possibly many) functions. It must provide the
// gradient of these element functions.

class bioIterativeFunction {

public:
  bioIterativeFunction() ;
  virtual patULong nbrOfElements() const = PURE_VIRTUAL ;
  virtual patVariables getCurrentGradient(patVariables& x,
					  patError*& err) = PURE_VIRTUAL ;
  virtual void first() = PURE_VIRTUAL ;
  // Circular next. When the end is reached, it starts again
  virtual void next() = PURE_VIRTUAL ;
};

#endif
