//-*-c++-*------------------------------------------------------------
//
// File name : bioPrematureStop.h
// Author :    Michel Bierlaire 
// Date :      Fri Aug  4 14:55:40 2017
//
//--------------------------------------------------------------------

// The stopping criterion is verified if the current iteration of the
// algorithm is close enough to a point already visited. (see
// Bierlaire et al, 2011 for details)

#ifndef bioPrematureStop_h
#define bioPrematureStop_h

#include <vector>
#include "patVariables.h"
#include "bioAlgorithmManager.h"
#include "patError.h"

class bioPrematureStop : public bioAlgorithmManager {
 public:
  bioPrematureStop(vector<patVariables>* aList, patError*& err) ;
  patBoolean interruptIterations() ;
  patString reasonForInterruption() ;
 private:
  patReal threshold ;
  vector<patVariables>* theList ;
  patVariables neighbor ;
};
#endif
