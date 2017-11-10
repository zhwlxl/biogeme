//-*-c++-*------------------------------------------------------------
//
// File name : bioMarkovTransition.h
// Author :    Michel Bierlaire
// Date :      Mon Aug  9 11:10:43 2010
//
//--------------------------------------------------------------------

#ifndef bioMarkovTransition_h
#define bioMarkovTransition_h

#include "patType.h"

/**
It captures the negihbouring state with its probability to be accessed.
 */

class bioMarkovState ;

class bioMarkovTransition {
 public:
  bioMarkovTransition() ;
  bioMarkovState* theCurrentState ;
  bioMarkovState* theNextState ;
  patReal theProbaForward ;
  patReal theProbaBackward ;
  patString name ;
  patString print() ;
};


#endif
