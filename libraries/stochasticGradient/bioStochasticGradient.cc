//-*-c++-*------------------------------------------------------------
//
// File name : bioStochasticGradient.cc
// Author :    Michel Bierlaire
// Date :      Mon Jul  3 20:28:37 2017
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "patDisplay.h"
#include "bioStochasticGradient.h"
#include "patErrNullPointer.h"
#include "patErrMiscError.h"
#include "bioIterativeFunction.h"

bioStochasticGradient::bioStochasticGradient(bioIterativeFunction* f, patError*& err): theFunction(f)
{
  if (theFunction == NULL) {
    err = new patErrNullPointer("bioIterativeFunction") ;
    WARNING(err->describe());
    return ;
  }
}
void bioStochasticGradient::init(patVariables x0, patError*& err) {
  currentIterate = x0 ;
}

patVariables bioStochasticGradient::getSolution() const {
  return currentIterate ;
}

void bioStochasticGradient::onePass(patError*& err) {
  theFunction->first() ;
  for (patULong i = 0 ; i < theFunction->nbrOfElements() ; ++i) {
    patVariables g = theFunction->getCurrentGradient(currentIterate,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return ;
    }
    listOfGradients.push_back(g) ;
    updateIterate(g) ;
    theFunction->next() ;
  }
}

void bioStochasticGradient::randomStep(patError*& err) {
  err = new patErrMiscError("Not yet implemented") ;
  WARNING(err->describe()) ;
  return ;
}
  
void bioStochasticGradient::updateIterate(patVariables& gradient) {
  
}
