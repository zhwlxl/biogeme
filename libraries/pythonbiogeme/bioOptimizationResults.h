//-*-c++-*------------------------------------------------------------
//
// File name : bioOptimizationResults.h
// Author :    Michel Bierlaire
// Date :      Wed Jun 28 19:08:54 2017
//
//--------------------------------------------------------------------

// TO DO: include the derivatives of the constraints 

#ifndef bioOptimizationResults_h
#define bioOptimizationResults_h

#include <map>
#include "patVariables.h"
#include "trVector.h"
#include "trHessian.h"

#include "trParameters.h"

class bioOptimizationResults {
public:
  bioOptimizationResults(patBoolean secondDerivatives = patFALSE, patULong n = 0) ;
  ~bioOptimizationResults() ;
public:
  patReal objective ;
  patVariables solution ;
  trVector gradient ;
  trHessian* hessian ;
  trHessian* bhhh ;
  map<patReal,patVariables> eigenVectors ;
  patString diagnostic ;
  patString runTime ;
  patULong iterations ;
  patMyMatrix getInverseHessian(patError*& err) ;
  patULong getSize() const;
  vector<patVariables> bootstrapSolutions ;
public:
  patBoolean readyForSecondDerivatives() const ;
  void prepareMemoryForMatrices(trParameters _theTrParameters, patError*& err) ;
  void cancelInverseCalculation() ;
  patBoolean isSandwichAvailable() const ;
  patReal getGradientNorm() ;
  patReal getSmallestSingularValue(patError*& err) ;
 private:
  patULong size ;
  patBoolean secondDerivatives ;
  patBoolean inverseComputed ;
  patMyMatrix inverseHessian ;
  patReal smallestSingularValue ;
 private:
  patBoolean compute(patError*& err) ;
  
};
#endif
