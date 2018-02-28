//-*-c++-*------------------------------------------------------------
//
// File name : bioStochasticGradientFunction
// Author :    Michel Bierlaire
// Date :      Sun Dec 17 16:09:17 2017
//
//--------------------------------------------------------------------


#ifndef bioStochasticGradientFunction_h
#define bioStochasticGradientFunction_h

#include "patError.h"
#include "patConst.h"
#include "bioFunctionAndDerivatives.h"

#include "trVector.h"



class bioStochasticGradientFunction {

public :

  virtual ~bioStochasticGradientFunction() {} ;
  virtual void init() = PURE_VIRTUAL ;
  virtual void next() = PURE_VIRTUAL ;
  virtual patBoolean isDone() = PURE_VIRTUAL ;
  virtual void random() = PURE_VIRTUAL ;
  // It is assumed that the items are numbered from 0 to getSize()-1
  virtual patULong getIndex() = PURE_VIRTUAL ;
  virtual patULong getSize() = PURE_VIRTUAL ;
  virtual bioFunctionAndDerivatives* getFunctionAndDerivatives(trVector* x,
							       patError*& err) = PURE_VIRTUAL ;

 protected:
  bioStochasticGradientFunction() {} 
};



#endif
