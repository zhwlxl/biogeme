//-*-c++-*------------------------------------------------------------
//
// File name : patFunctionAuto.h
// Author :    \URL[Michel Bierlaire]{http://rosowww.epfl.ch/mbi}
// Date :      
//
//--------------------------------------------------------------------

#ifndef patFunctionAuto_h
#define patFunctionAuto_h

#include "patFunction.h"
#include "patVariables.h"
#include "patError.h"

/**
   @doc Defines an interface for functions $f:\mathbb{R}^n \rightarrow \mathbb{R}^n$, in order to tests fixed point algorithms
   @author \URL[Michel Bierlaire]{http://rosowww.epfl.ch/mbi}
 */
class patFunctionAuto: public patFunction {

 public: 
  /**
   */
  virtual ~patFunctionAuto() {}
  /**
     Purely virtual
   */
  virtual patVariables getImage(const patVariables& x, patError*) 
    = PURE_VIRTUAL ;
  /**
   */
  virtual patVariables getSource(patError*) = PURE_VIRTUAL ;
};


#endif
