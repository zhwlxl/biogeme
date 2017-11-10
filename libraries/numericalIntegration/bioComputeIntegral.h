//-*-c++-*------------------------------------------------------------
//
// File name : bioComputeIntegral.h
// Author :    Michel Bierlaire
// Date :      Thu Feb 27 09:58:21 2014
//
//--------------------------------------------------------------------

#ifndef bioComputeIntegral_h
#define bioComputeIntegral_h

#include "patError.h"
#include "patType.h"
#include "bioMoriFunction.h"
class bioComputeIntegral {
public:
  bioComputeIntegral(bioMoriFunction* f) ;
  bioComputeIntegral(bioMoriFunction* f, patReal l, patReal u) ;
  patReal compute(patError*& err) ;
 private:
  patBoolean lowerInfinity ;
  patReal lowerBound ;
  patBoolean upperInfinity ;
  patReal upperBound ;
  bioMoriFunction *theFunction ;

};
#endif
