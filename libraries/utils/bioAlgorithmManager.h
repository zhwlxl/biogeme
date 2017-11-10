//-*-c++-*------------------------------------------------------------
//
// File name : bioAlgorithmManager.h
// Author :    Michel Bierlaire 
// Date :      Fri Aug  4 10:15:21 2017
//
//--------------------------------------------------------------------

#ifndef bioAlgorithmManager_h
#define bioAlgorithmManager_h

#include "patConst.h"
#include "patType.h"
#include "patVariables.h"

class bioAlgorithmManager {

 public:
  virtual ~bioAlgorithmManager() ;
  virtual patBoolean interruptIterations() = PURE_VIRTUAL ;
  virtual patString reasonForInterruption() = PURE_VIRTUAL ;
  virtual void setCurrentIterate(const patVariables& x) ;
  virtual void setCurrentFunction(patReal f) ;
  virtual void setCurrentGradient(const patVariables& g) ;
 protected:
  patVariables x ;
  patReal f ;
  patVariables g ;
};
#endif

