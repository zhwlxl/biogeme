//-*-c++-*------------------------------------------------------------
//
// File name : patAlgorithms.h
// Author :    \URL[Michel Bierlaire]{http://rosowww.epfl.ch/mbi}
// Date :      Wed Mar  3 20:17:26 1999
//
//--------------------------------------------------------------------

#ifndef patAlgorithms_h
#define patAlgorithms_h

#include "patConst.h"
#include "patVariables.h"

class patFunction ;
class patError ;

/**
   @doc Virtual class defining a common interface for derivative-free algorithms
   @author \URL[Michel Bierlaire]{http://rosowww.epfl.ch/mbi}, EPFL (Wed Mar  3 20:17:26 1999)
*/
class patAlgorithms {
public:
  /**
   */
  patAlgorithms(patFunction* f = NULL) : functionPtr(f) {}
  /**
   */
  virtual ~patAlgorithms() {}
  /**
     Purely virtual 
   */
  virtual string getName() = PURE_VIRTUAL ;
  /**
     Purely virtual 
   */
  virtual string getDescription() = PURE_VIRTUAL ;
  /**
   */
  virtual void setFunction(patFunction* f) { functionPtr = f ;}
  /**
   */
  virtual patFunction* getFunction() { return functionPtr ;}
  /**
     Returns the value obtained after a givenm number of function evaluations. Used to compare algorithms efficiency. Purely virtual 
   */
  virtual patReal getValueAfterNFctEval(unsigned long n) = PURE_VIRTUAL ;
  /**
     Purely virtual 
   */
  virtual void run(patError*& ptr) = PURE_VIRTUAL ;
  /**
     Purely virtual 
   */
  virtual void setFirstIter(const patVariables& x,
			    patError*& ptr) 
    = PURE_VIRTUAL ;
  /**
     Purely virtual 
     @return patTRUE if the algorithms may produce different results with the same initial solution.
   */
  virtual patBoolean isRandomized() = PURE_VIRTUAL ;
  
protected:
  patFunction* functionPtr ;
};
#endif
