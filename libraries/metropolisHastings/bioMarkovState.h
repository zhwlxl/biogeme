//-*-c++-*------------------------------------------------------------
//
// File name : bioMarkovState.h
// Author :    Michel Bierlaire
// Date :      Mon Aug  9 11:02:21 2010
//
//--------------------------------------------------------------------

#ifndef bioMarkovState_h
#define bioMarkovState_h

#include "patType.h"
#include "patConst.h"
#include "patError.h"

/**
This virtual object represents a state of a Markov process. 
*/

class bioMarkovState {

 public:
  bioMarkovState() ;
  // The weight is the unnormalized target probability associated with
  // the state
  virtual patReal getWeight(patError*& err) const = PURE_VIRTUAL ;
  // The indicator is used to collect statistics and, in particular,
  // to test for stationarity. If there is a small number of states,
  // it could be the identifier of the state. If there is a large
  // number, another value can be used.
  // IN any case it must be unique.
  virtual patULong getIndicator(patError*& err) const = PURE_VIRTUAL ;

  virtual patString print() const = PURE_VIRTUAL;
};
#endif
