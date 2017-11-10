//-*-c++-*------------------------------------------------------------
//
// File name : patFixedPointAlgorithms.h
// Author :    \URL[Michel Bierlaire]{http://rosowww.epfl.ch/mbi}
// Date :      Wed Jun  9 22:53:23 1999
//
//--------------------------------------------------------------------

#ifndef patFixedPointAlgorithms_h
#define patFixedPointAlgorithms_h

#include "patConst.h"
#include "patVariables.h"

class patFunctionAuto ;
class patError ;

/**
 @doc Virtual class defining a common interface for fixed point algorithms
 @author \URL[Michel Bierlaire]{http://rosowww.epfl.ch/mbi} (Wed Jun  9 22:53:23 1999)
*/
class patFixedPointAlgorithms : public patAlgorithms {
public:
  /**
     @param f pointer to the function
   */
  patFixedPointAlgorithms(patFunctionAuto* f = NULL) : functionPtr(f), patAlgorithms(f) {}
  /**
   */
  virtual ~patFixedPointAlgorithms() {}
  /**
   */
  virtual void setFunction(patFunctionAuto* f) { functionPtr = f ;}
  /**
   */
  virtual patFunctionAuto* getFunction() { return functionPtr ;}
  
protected:
  patFunctionAuto* functionPtr ;
};
#endif
