//-*-c++-*------------------------------------------------------------
//
// File name : bioDraftSG.cc
// Author :    Michel Bierlaire
// Date :      Fri Dec 15 11:21:16 2017
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "bioDraftSG.h"
#include "bioExpressionRepository.h"
#include "bioSample.h"
#include "bioLiteralRepository.h"
#include "patErrNullPointer.h"

bioDraftSG::bioDraftSG(bioExpressionRepository* rep, 
		       patULong expId,
		       bioSample* sample,
		       patError*& err) 
  : theExpressionId(expId),
    theSample(sample),
    theSpan(patString("SingleRowForStochasticGradient"),0,1),
    debugDeriv(patFALSE) {
  
  theExpression = rep->getExpression(theExpressionId) ;
  theExpression->checkMonteCarlo(patFALSE,err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return ;
  }
  
  if (!theExpression->isSumIterator()) {
    err = new patErrMiscError("Stochastic gradient can be applied only with an sum iterator.") ;
    WARNING(err->describe()) ;
    return  ;
  }

  betaIds = bioLiteralRepository::the()->getBetaIds(patFALSE, err)  ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return ;
  }

  if (theSample == NULL) {
    err = new patErrNullPointer("bioSample") ;
    WARNING(err->describe()) ;
    return ;
  }
  sampleSize = theSample->size() ;

  theExpression->setSample(theSample) ;

#ifdef DEBUG
  debugDeriv = (bioParameters::the()->getValueInt("debugDerivatives",err) != 0) ;
#endif 

}

bioDraftSG::~bioDraftSG() {
  
}

void bioDraftSG::init() {
  theSpan.firstRow = 0 ;
  theSpan.lastRow = theSpan.firstRow + 1 ;
}

void bioDraftSG::next() {
  ++theSpan.firstRow ;
  ++theSpan.lastRow ;
}

patBoolean bioDraftSG::isDone() {
  return (theSpan.lastRow >= sampleSize) ;
}


bioFunctionAndDerivatives* bioDraftSG::getFunctionAndDerivatives(trVector* x, patError*& err) {
  theExpression->setThreadSpan(theSpan);
  bioFunctionAndDerivatives* result = 
    theExpression->getNumericalFunctionAndGradient(betaIds,
						   patFALSE,
						   debugDeriv,
						   err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return NULL ;
  }
  return result ;

}

void bioDraftSG::random() {
  theSpan.firstRow =  getSize() * ( rand() / RAND_MAX) ;
  theSpan.lastRow = theSpan.firstRow + 1 ;
}

patULong bioDraftSG::getIndex() {
  return theSpan.firstRow ;
}

patULong bioDraftSG::getSize() {
  return sampleSize ;
}
