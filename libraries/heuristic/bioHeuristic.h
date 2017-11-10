//-*-c++-*------------------------------------------------------------
//
// File name : bioHeuristic.h
// Author :    Michel Bierlaire 
// Date :      Fri Aug  4 17:57:05 2017
//
//--------------------------------------------------------------------

#ifndef bioHeuristic_h
#define bioHeuristic_h

#include "trNonLinearAlgo.h"

class bioHeuristic: public trNonLinearAlgo {

 public:
  bioHeuristic(trNonLinearAlgo* ls,
	       patULong maxIter,bioAlgorithmManager* sc) ;
  patString run(patError*& err) ;
  patULong nbrIter() ;
  patVariables getSolution(patError*& err)  ;
  patReal getValueSolution(patError*& err) ;
  patVariables getLowerBoundsLambda()  ;
  patVariables getUpperBoundsLambda()  ;
  void defineStartingPoint(const patVariables& x0)  ;
  patString getName()  ;
  
 private:
  trNonLinearAlgo* localSearch ;
  vector<patVariables> listOfLocalOptima ;
  vector<patReal> valuesOfLocalOptima ;
  patULong maximumNumberOfLocalSearch ;
  
};
#endif
