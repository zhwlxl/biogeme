//-*-c++-*------------------------------------------------------------
//
// File name : bioSaga.cc
// Author :    Michel Bierlaire
// Date :      Wed Feb 14 15:12:35 2018
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "patDisplay.h"
#include "bioSaga.h"
#include "bioStochasticGradientFunction.h"
#include "patErrNullPointer.h"
#include "patErrOutOfRange.h"
#include "patErrMiscError.h"
#include "bioParameters.h"

#include "bioFunctionAndDerivatives.h"

bioSaga::bioSaga(bioStochasticGradientFunction* f,
		 patError*& err): theFunction(f)
{
  if (theFunction == NULL) {
    err = new patErrNullPointer("bioIterativeFunction") ;
    WARNING(err->describe());
    return ;
  }

  listOfGradients.resize(theFunction->getSize()) ;

  stepSize = bioParameters::the()->getValueReal("sagaStepSize",err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return ;
  }
  numberOfPasses = bioParameters::the()->getValueInt("sagaNumberOfPasses",err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return ;
  }
}
void bioSaga::init(patVariables x0, patError*& err) {
  currentIterate = x0 ;
}

patVariables bioSaga::getSolution() const {
  return currentIterate ;
}

void bioSaga::run(patError*& err) {
  DEBUG_MESSAGE("First: " << currentIterate) ;
  initGradients(err) ;
  DEBUG_MESSAGE("After init: " << currentIterate) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return  ;
  }
  for (patULong i = 1 ;
       i <= numberOfPasses ;
       ++i) {
    GENERAL_MESSAGE("Stochastic gradient: pass " << i) ;
    for (patULong k = 0 ;
	 k <= listOfGradients.size() ;
	 ++k) {
      theFunction->random() ;
      patULong index = theFunction->getIndex() ;
      if (index >= listOfGradients.size()) {
	err = new patErrOutOfRange<patULong>(index,0,listOfGradients.size()-1) ;
	WARNING(err->describe()) ;
	return ;
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
      patVariables d = theFg->theGradient - listOfGradients[index] + totalGradient / patReal(listOfGradients.size()) ;
      totalGradient -= listOfGradients[index] ;
      listOfGradients[index] = theFg->theGradient ;
      totalGradient += listOfGradients[index] ;
      updateIterate(d) ;
    }
    DEBUG_MESSAGE("After pass " << i << ": " << currentIterate) ;
  }
}


void bioSaga::updateIterate(patVariables& direction) {

  // We maximize.
  DEBUG_MESSAGE("xk = " << currentIterate) ;
  DEBUG_MESSAGE("Direction: " << direction << " Norm: " << norm2(direction)) ;
  currentIterate += stepSize * direction ;
  DEBUG_MESSAGE("xk+1 = " << currentIterate) ;
}

  
void bioSaga::updateIterateScaled(patVariables& gradient,patReal gradientScale) {
  
  DEBUG_MESSAGE("-> xk = " << currentIterate) ;
  DEBUG_MESSAGE("-> Direction: " << gradientScale << " * " << gradient << " Norm: " << gradientScale * norm2(gradient)) ;
  currentIterate += stepSize * gradientScale * gradient ;
  DEBUG_MESSAGE("-> xk+1 = " << currentIterate) ;
}

patVariables bioSaga::getSolution() {
  return currentIterate ;
}



void bioSaga::initGradients(patError*& err) {
  if (theFunction == NULL) {
    err = new patErrNullPointer("bioIterativeFunction") ;
    WARNING(err->describe());
    return ;
  }


  patULong count = 0  ;

  for (theFunction->init() ;
       !theFunction->isDone() ;
       theFunction->next()) {
    ++count ;
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
    patULong index = theFunction->getIndex() ;
    if (index >= listOfGradients.size()) {
      err = new patErrOutOfRange<patULong>(index,0,listOfGradients.size()-1) ;
      WARNING(err->describe()) ;
      return ;
    }
    listOfGradients[index] = theFg->theGradient ;
    if (count == 1) {
      totalGradient = theFg->theGradient ;
    }
    else {
      totalGradient += theFg->theGradient ;
    }
    updateIterateScaled(totalGradient,1.0/patReal(count)) ;
  }
}
