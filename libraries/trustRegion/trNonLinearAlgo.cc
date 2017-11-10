//-*-c++-*------------------------------------------------------------
//
// File name : trNonLinearAlgo.cc
// Author :    Michel Bierlaire
// Date :      Fri Aug 31 11:50:10 2001
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "trNonLinearAlgo.h"
#include "patNonLinearProblem.h"
#include "patDisplay.h"
#include "patIterationBackup.h"
#include "bioAlgoStopFile.h"

trNonLinearAlgo::trNonLinearAlgo(patNonLinearProblem* aProblem, bioAlgorithmManager* aStoppingCriteria) :
  theProblem(aProblem), theBackup(NULL), theStoppingCriteria(aStoppingCriteria) {
  if (aProblem != NULL) {
    //    DETAILED_MESSAGE("Create algorithm to solve " << aProblem->getProblemName()) ;
  } 
  else {
    WARNING("No problem specified") ;
  }
  if (theStoppingCriteria == NULL) {
    WARNING("No stopping criteria has been specified. The algorithm can be interrupted if a file named STOP is created in the working directory") ;
    theStoppingCriteria = new bioAlgoStopFile("STOP") ;
  }
}

trNonLinearAlgo::~trNonLinearAlgo() {

}

void trNonLinearAlgo::setBackup(patIterationBackup* aBackup) {
  theBackup = aBackup ;
}

patBoolean trNonLinearAlgo::isAvailable() const {
  return patTRUE ;
}
