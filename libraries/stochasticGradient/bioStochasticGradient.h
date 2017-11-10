//-*-c++-*------------------------------------------------------------
//
// File name : bioStochasticGradient.h
// Author :    Michel Bierlaire
// Date :      Mon Jul  3 20:21:37 2017
//
//--------------------------------------------------------------------

#ifndef bioStochasticGradient_h
#define bioStochasticGradient_h

#include <vector>
#include "patError.h"
#include "patVariables.h"

class bioIterativeFunction ;

class bioStochasticGradient {
 public:
  bioStochasticGradient(bioIterativeFunction* f, patError*& err) ;
  void init(patVariables x0, patError*& err) ;
  patVariables getSolution() const ;
  void onePass(patError*& err) ;
  void randomStep(patError*& err) ;
  
 private:
  void updateIterate(patVariables& gradient) ;
 private:
  bioIterativeFunction* theFunction ;
  patVariables currentIterate ;
  vector<patVariables> listOfGradients ;
};
#endif
