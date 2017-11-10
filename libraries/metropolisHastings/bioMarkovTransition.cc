//-*-c++-*------------------------------------------------------------
//
// File name : bioMarkovTransition.cc
// Author :    Michel Bierlaire
// Date :      Mon Aug  9 15:03:05 2010
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <sstream>
#include "bioMarkovTransition.h"
#include "bioMarkovState.h"

bioMarkovTransition::bioMarkovTransition() : theCurrentState(NULL), theNextState(NULL){

}

patString bioMarkovTransition::print() {
  stringstream str ;
  str << name << '\t' ;
  str << theCurrentState->print() << '\t' ;
  str << theNextState->print() << '\t'  ;
  str << theProbaForward << '\t' ;
  str << theProbaBackward ;
  return patString(str.str()) ;
}
