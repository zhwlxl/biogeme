//-*-c++-*------------------------------------------------------------
//
// File name : bioMoriFunction.h
// Author :    Michel Bierlaire
// Date :      Thu Feb 27 09:53:50 2014
//
//--------------------------------------------------------------------

#ifndef bioMoriFunction_h
#define bioMoriFunction_h

#include "patError.h"
class bioMoriFunction {
 public:
  virtual patReal operator()(patReal x, patError*& err) const = PURE_VIRTUAL ;
};
#endif 
