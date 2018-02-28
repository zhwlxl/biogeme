//-*-c++-*------------------------------------------------------------
//
// File name : bioSaga.h
// Author :    Michel Bierlaire
// Date :      Wed Feb 14 14:44:10 2018
//
//--------------------------------------------------------------------

// Implement the algorithm proposed by:
// Defazio, A., Bach, F. and Lacoste-Julien, S., 2014. SAGA: A fast
// incremental gradient method with support for non-strongly convex
// composite objectives. In Advances in neural information processing
// systems (pp. 1646-1654).

#ifndef bioSaga_h
#define bioSaga_h

#include <vector>
#include "patError.h"
#include "patVariables.h"
#include "patIteratorErr.h"

class bioStochasticGradientFunction ;

class bioFunctionAndDerivatives ;

class bioSaga {
 public:
  bioSaga(bioStochasticGradientFunction* f, patError*& err) ;
  void init(patVariables x0, patError*& err) ;
  void run(patError*& err) ;
  patVariables getSolution() const ;
  patVariables getSolution() ;
 private:
  void onePass(patError*& err) ;
  void updateIterate(patVariables& gradient) ;
  void updateIterateScaled(patVariables& gradient,patReal scale) ;
  void initGradients(patError*& err) ;
 private:
  bioStochasticGradientFunction* theFunction ;
  patVariables currentIterate ;
  vector<patVariables> listOfGradients ;
  patVariables totalGradient ;
  patReal stepSize ;
  
  patULong numberOfPasses ;
};

#endif
