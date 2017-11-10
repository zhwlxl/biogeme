//-*-c++-*------------------------------------------------------------
//
// File name : bioPathMarkovProcess.h
// Author :    Michel Bierlaire
// Date :      Wed Aug 11 17:33:01 2010
//
//--------------------------------------------------------------------

#ifndef bioPathMarkovProcess_h
#define bioPathMarkovProcess_h

#include "bioMarkovProcess.h"

class bioPathMarkovProcess : public bioMarkovProcess {

  virtual bioMarkovTransition getNextState(patError*& err)  ;

} ;
#endif
