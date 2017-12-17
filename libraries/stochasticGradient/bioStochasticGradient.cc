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
#include "bioStochasticGradientFunction.h"
#include "patErrNullPointer.h"
#include "patErrMiscError.h"
#include "bioParameters.h"

#include "bioFunctionAndDerivatives.h"

bioStochasticGradient::bioStochasticGradient(bioStochasticGradientFunction* f,
					     patError*& err): theFunction(f)
{
  if (theFunction == NULL) {
    err = new patErrNullPointer("bioIterativeFunction") ;
    WARNING(err->describe());
    return ;
  }
  stepSize = bioParameters::the()->getValueReal("stochasticGradientFirstStepSize",err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return ;
  }
  stepSizeFactor = bioParameters::the()->getValueReal("stochasticGradientStepUpdate",err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return ;
  }
  numberOfPasses = bioParameters::the()->getValueInt("stochasticGradientNumberOfPasses",err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return ;
  }
}
void bioStochasticGradient::init(patVariables x0, patError*& err) {
  currentIterate = x0 ;
}

patVariables bioStochasticGradient::getSolution() const {
  return currentIterate ;
}

void bioStochasticGradient::run(patError*& err) {
  for (patULong i = 1 ;
       i <= numberOfPasses ;
       ++i) {
    GENERAL_MESSAGE("Stochastic gradient: pass " << i) ;
    onePass(err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return  ;
    }
  }
}


void bioStochasticGradient::onePass(patError*& err) {
  if (theFunction == NULL) {
    err = new patErrNullPointer("bioIterativeFunction") ;
    WARNING(err->describe());
    return ;
  }
  for (theFunction->init() ;
       !theFunction->isDone() ;
       theFunction->next()) {
    if (err != NULL) {
      WARNING(err->describe()) ;
      return  ;
    }
    bioFunctionAndDerivatives* theFg = theFunction->getFunctionAndDerivatives(&currentIterate,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return ;
    }
    if (theFg == NULL) {
      err = new patErrNullPointer("bioFunctionAndDerivatives") ;
      WARNING(err->describe()) ;
      return ;
    }
    updateIterate(theFg->theGradient) ;
  }
}

  
void bioStochasticGradient::updateIterate(patVariables& gradient) {
  
  currentIterate += stepSize * gradient ;
    stepSize *= stepSizeFactor ;
}

patVariables bioStochasticGradient::getSolution() {
  return currentIterate ;
}

