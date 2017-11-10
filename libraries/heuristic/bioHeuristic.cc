//-*-c++-*------------------------------------------------------------
//
// File name : bioHeuristic.cc
// Author :    Michel Bierlaire 
// Date :      Fri Aug  4 18:06:24 2017
//
//--------------------------------------------------------------------
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
