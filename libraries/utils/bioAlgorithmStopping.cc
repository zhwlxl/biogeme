//-*-c++-*------------------------------------------------------------
//
// File name : bioAlgorithmStopping.cc
// Author :    Michel Bierlaire 
// Date :      Fri Aug  4 10:27:03 2017
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "bioAlgorithmStopping.h"

bioAlgorithmStopping::bioAlgorithmStopping(bioAlgorithmManager* s) {
  if (s != NULL) {
    theStoppers.push_back(s) ;
  }
}
void bioAlgorithmStopping::addStoppingCriterion(bioAlgorithmManager* s) {
  if (s != NULL) {
    theStoppers.push_back(s) ;
  }
}

patBoolean bioAlgorithmStopping::interruptIterations() {
  for (vector<bioAlgorithmManager*>::iterator i = theStoppers.begin() ;
       i != theStoppers.end() ;
       ++i) {
    if ((*i)->interruptIterations()) {
      reason = (*i)->reasonForInterruption() ;
      return patTRUE ;
    }
  }
  return patFALSE ;
}

patString bioAlgorithmStopping::reasonForInterruption() {
  return reason ;
}

void bioAlgorithmStopping::setCurrentIterate(const patVariables& x) {
  for (vector<bioAlgorithmManager*>::iterator i = theStoppers.begin() ;
       i != theStoppers.end() ;
       ++i) {
    (*i)->setCurrentIterate(x) ;
  }

}
void bioAlgorithmStopping::setCurrentFunction(patReal f) {
  for (vector<bioAlgorithmManager*>::iterator i = theStoppers.begin() ;
       i != theStoppers.end() ;
       ++i) {
    (*i)->setCurrentFunction(f) ;
  }

}
void bioAlgorithmStopping::setCurrentGradient(const patVariables& g) {
  for (vector<bioAlgorithmManager*>::iterator i = theStoppers.begin() ;
       i != theStoppers.end() ;
       ++i) {
    (*i)->setCurrentGradient(g) ;
  }

}
