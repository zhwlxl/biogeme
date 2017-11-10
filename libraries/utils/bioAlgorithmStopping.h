//-*-c++-*------------------------------------------------------------
//
// File name : bioAlgorithmStopping.h
// Author :    Michel Bierlaire 
// Date :      Fri Aug  4 10:24:43 2017
//
//--------------------------------------------------------------------

#ifndef bioAlgorithmStopping_h
#define bioAlgorithmStopping_h

#include <vector>
#include "bioAlgorithmManager.h"

class bioAlgorithmStopping : public bioAlgorithmManager {
 public:
  bioAlgorithmStopping(bioAlgorithmManager* s = NULL) ;
  void addStoppingCriterion(bioAlgorithmManager* s) ;
  patBoolean interruptIterations() ;
  patString reasonForInterruption() ;
  void setCurrentIterate(const patVariables& x) ;
  void setCurrentFunction(patReal f) ;
  void setCurrentGradient(const patVariables& g) ;

 private:
  vector<bioAlgorithmManager*> theStoppers ;
  patString reason ;
};
#endif
