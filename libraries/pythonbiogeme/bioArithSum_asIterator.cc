//-*-c++-*------------------------------------------------------------
//
// File name : bioArithSum.cc
// Author :    Michel Bierlaire and Mamy Fetiarison
// Date :      Tue Jun 16 12:47:18  2009
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sstream>

#include "patMath.h"
#include "patDisplay.h"
#include "patErrNullPointer.h"
#include "patErrMiscError.h"
#include "patError.h"

#include "trHessian.h"
#include "trParameters.h"

#include "bioRowIterator.h"
#include "bioMetaIterator.h"
#include "bioDrawIterator.h"
#include "bioLiteralRepository.h"
#include "bioParameters.h"
#include "bioRandomDraws.h"
#include "bioArithCompositeLiteral.h"

#include "bioIteratorInfoRepository.h"
#include "bioSample.h"

#include "bioArithSum.h"
#include "bioArithBinaryPlus.h"
#include "bioLiteralRepository.h"
#include "bioExpressionRepository.h"

/*!
*/
bioArithSum::bioArithSum(bioExpressionRepository* rep,
			 patULong par,
                         patULong left,
                         patString it,
			 patULong w,
			 patError*& err) 
  : bioArithIterator(rep, par,left,it,err),bhhh(NULL),weightId(w),weight(NULL),accessToFirstRow(patBadId), scaleDerivatives(patFALSE), theScale(1.0), theRowIter(NULL), theMetaIter(NULL), _literalIds(NULL) {

  if (err != NULL) {
    WARNING(err->describe()) ;
  }
  
  weight = theRepository->getExpression(w) ;

  


}

bioArithSum::~bioArithSum() {}

patString bioArithSum::getOperatorName() const {
  return patString("Sum") ;
}

bioExpression* bioArithSum::getDerivative(patULong aLiteralId, patError*& err) const {

  if (child == NULL) {
    err = new patErrNullPointer("bioExpression") ;
    WARNING(err->describe()) ;
    return NULL;
  }
  
  bioExpression* leftResult = child->getDerivative(aLiteralId,err) ;

  if (err != NULL) {
    WARNING(err->describe()) ;
    return NULL;
  }

  bioExpression* result = new bioArithSum(theRepository,patBadId,leftResult->getId(),theIteratorName,weightId,err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return NULL;
  }
  return result ;
}


bioArithSum* bioArithSum::getDeepCopy(bioExpressionRepository* rep,
				      patError*& err) const {
  bioExpression* leftClone(NULL) ;
  if (child != NULL) {
    leftClone = child->getDeepCopy(rep,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return NULL ;
    }
  }
  bioArithSum* theNode = 
    new bioArithSum(rep,patBadId,leftClone->getId(),theIteratorName,weightId,err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return NULL ;
  }
  return theNode ;
}

bioArithSum* bioArithSum::getShallowCopy(bioExpressionRepository* rep,
					 patError*& err) const {
  bioArithSum* theNode = 
    new bioArithSum(rep,patBadId,child->getId(),theIteratorName,weightId,err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return NULL ;
  }
  return theNode ;
}



patBoolean bioArithSum::isSum() const {
  return patTRUE ;
}

patBoolean bioArithSum::isProd() const {
  return patFALSE ;
}

patString bioArithSum::getExpressionString() const {
  stringstream str ;
  str << "$S" ;
  str << theIteratorName ;
  if (child != NULL) {
    str << '{' << child->getExpressionString() << '}' ;
  }
  return patString(str.str()) ;
}

patBoolean bioArithSum::isSumIterator() const {
  return patTRUE ;
}




patReal bioArithSum::getValue(patBoolean prepareGradient, patULong currentLap, patError*& err)  {

  if(currentLap > lastComputedLap || currentLap == patLapForceCompute){

    if (child->getId() != childId) {
      stringstream str ;
      str << "Ids are different: " 
  	<< child->getId() 
  	<< " <> " 
  	<< childId ;
      err = new patErrMiscError(str.str()) ;
      WARNING(err->describe()) ;
      return patReal() ;
    }
    
    
    if (theSample == NULL) {
      err = new patErrNullPointer("bioSample") ;
      WARNING(err->describe()) ;
      return patReal() ;
    }
    patReal result = 0.0 ;
    
    if(bhhh != NULL) {
      bhhh->setToZero() ;
    }

    if (theIteratorType == ROW) {
      bioRowIterator* theIter = theSample->createRowIterator(theCurrentSpan,
  							   theThreadSpan,
  							   patFALSE,
  							   err) ;
      if (err != NULL) {
        WARNING(err->describe()) ;
        return patReal() ;
      }
      nIter = 0 ;
      for (theIter->first() ; !theIter->isDone() ; theIter->next()) {
        ++nIter ;
        child->setVariables(theIter->currentItem()) ;
        patReal expression = child->getValue(patFALSE, patLapForceCompute, err) ;
        if (err != NULL) {
        	WARNING(err->describe()) ;
        	return patReal() ;
        }
	
        if (weight != NULL && isTop()) {
        	weight->setVariables(theIter->currentItem()) ;
        	patReal w = weight->getValue(patFALSE, patLapForceCompute, err) ;
        	if (err != NULL) {
        	  WARNING(err->describe()) ;
        	  return patReal() ;
        	}
          result += w * expression ;
        } else {
          result += expression ;
        }
      }

      DELETE_PTR(theIter) ;
      //return result ;
    
    } else if (theIteratorType == META) {
      bioMetaIterator* theIter = theSample->createMetaIterator(theCurrentSpan,theThreadSpan,err) ;
      if (err != NULL) {
        WARNING(err->describe()) ;
        return patReal() ;
      }
      for (theIter->first() ; !theIter->isDone() ; theIter->next()) {
        child->setCurrentSpan(theIter->currentItem()) ;
	if (accessToFirstRow == patBadId) {
	  accessToFirstRow = bioParameters::the()->getValueInt("accessFirstDataFromMetaIterator",err) ;
	  if (accessToFirstRow) {
	    child->setVariables(theIter->getFirstRow()) ;
	  }
	}
        patReal expression = child->getValue(patFALSE, patLapForceCompute, err) ;
        if (err != NULL) {
        	WARNING(err->describe()) ;
        	return patReal() ;
        }

        result += expression ;
      }
      DELETE_PTR(theIter) ;
      //return result ;
    
    } else if (theIteratorType == DRAW) {
      err = new patErrMiscError("Deprecated code.") ;
      WARNING(err->describe()) ;
      return patReal() ;
      
    }else{
      err = new patErrMiscError("Should never be reached") ;
      WARNING(err->describe()) ;
      return patReal() ;
    }

    lastValue = result;
    lastComputedLap = currentLap;
    return lastValue ;
  }else{
    return lastValue;
  }

}


trHessian* bioArithSum::getBhhh() {
  return bhhh ;
}


patULong bioArithSum::getNumberOfOperations() const {
  patError* err(NULL) ;
    
  patULong result = 0 ;
    
    
  if (theIteratorType == ROW) {
    bioRowIterator* theIter = theSample->createRowIterator(theCurrentSpan,theThreadSpan,patFALSE,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return patReal() ;
    }
    for (theIter->first() ;
	 !theIter->isDone() ;
	 theIter->next()) {
      result += child->getNumberOfOperations() ;
    }
    DELETE_PTR(theIter) ;
    return result ;
  }
  else if (theIteratorType == META) {
    bioMetaIterator* theIter = theSample->createMetaIterator(theCurrentSpan,theThreadSpan,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return patReal() ;
    }
    for (theIter->first() ;
	 !theIter->isDone() ;
	 theIter->next()) {
      child->setCurrentSpan(theIter->currentItem()) ;
      result += child->getNumberOfOperations() ;
    }
    DELETE_PTR(theIter) ;
    return result ;
  }
  else if (theIteratorType == DRAW) {
    err = new patErrMiscError("Deprecated code.") ;
    WARNING(err->describe()) ;
    return patReal() ;
  }
  err = new patErrMiscError("Should never be reached") ;
  WARNING(err->describe()) ;
  return patReal() ;
}


bioFunctionAndDerivatives* bioArithSum::
  getNumericalFunctionAndGradient(vector<patULong> literalIds,
				  patBoolean computeHessian,
				  patBoolean debugDeriv,
				  patError*& err) {

  _literalIds = &literalIds ;
  _computeHessian = computeHessian ;
  _debugDeriv = debugDeriv ;
  for (first(err) ;
       !isDone(err) ;
       next(err)) {
    if (err != NULL) {
      WARNING(err->describe()) ;
      return NULL ;
    }
    bioFunctionAndDerivatives* theFg = currentItem(err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return NULL ;
    }
    if ( result.theFunction < patMaxReal) {
      if (theFg->theFunction == patMaxReal) {
	result.theFunction = patMaxReal ;
      }
      else {
	result.theFunction += theFg->theFunction ;
      }
    }
    for (patULong i = 0 ; i < literalIds.size() ; ++i) {
      patReal term(theFg->theGradient[i]) ;
      if (scaleDerivatives && theScale != 1.0) {
	term /= theScale ;
      }
      gradient[i] += term ;
    }
    if (computeHessian) {
      for (patULong i = 0 ; i < literalIds.size() ; ++i) {
	for (patULong j = i ; j < literalIds.size() ; ++j) {
	  patReal elem = theFg->theHessian.getElement(i,j,err) ;
	  if (err != NULL) {
	    stringstream str ;
	    str << "Row " << theRow << ": " << err->describe() ; 
	    DELETE_PTR(err) ;
	    err = new patErrMiscError(str.str()) ;
	    WARNING(err->describe()) ;
	    return NULL ;
	  }
	  if (scaleDerivatives && theScale != 1.0) {
	    elem /= theScale ;
	  }
	  result.theHessian.addElement(i,j,elem,err) ;
	  if (err != NULL) {
	    stringstream str ;
	    str << "Row " << theRow << ": " << err->describe() ; 
	    DELETE_PTR(err) ;
	    err = new patErrMiscError(str.str()) ;
	    WARNING(err->describe()) ;
	    return NULL ;
	  }
	}
      }
    }

    if (bhhh != NULL) {
      for (patULong i = 0 ; i < literalIds.size() ; ++i) {
	for (patULong j = i ; j < literalIds.size() ; ++j) {
	  if (weight != NULL && isTop()) {

	    // This is the reason why the weight has to be handled
	    // separately. It only needs to be multipled once for
	    // the computation of the BHHH.
	    bhhh->addElement(i,j,theFg->theGradient[i] * theFg->theGradient[j] / theWeightValue,err) ;
	  }
	  else {
	    bhhh->addElement(i,j,theFg->theGradient[i]*theFg->theGradient[j],err) ;
	  }
	  if (err != NULL) {
	    stringstream str ;
	    str << "Row " << theRow << ": " << err->describe() ; 
	    DELETE_PTR(err) ;
	    err = new patErrMiscError(str.str()) ;
	    WARNING(err->describe()) ;
	    return NULL ;
	  }
	}
      }
    }
  }

  if (scaleDerivatives && theScale != 1.0) {
    for (patULong i = 0 ; i < literalIds.size() ; ++i) {
      result.theGradient[i] = gradient[i] * theScale ;
    }
    result.theHessian.multAllEntries(theScale,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return NULL ;
    }
  }
  else {
    for (patULong i = 0 ; i < literalIds.size() ; ++i) {
      result.theGradient[i] = gradient[i] ;
    }    
  }

#ifdef  DEBUG
  if (debugDeriv != 0) {
    bioFunctionAndDerivatives* findiff = 
      getNumericalFunctionAndFinDiffGradient(literalIds, err)  ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return NoULL ;
    }
    patReal compare = result.compare(*findiff,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return NULL ;
    }
    
    patReal tolerance = bioParameters::the()->getValueReal("toleranceCheckDerivatives",err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return NULL ;
    }
    
    if (compare >= tolerance) {
      DEBUG_MESSAGE("Analytical: " << result) ;
      DEBUG_MESSAGE("FinDiff   : " << *findiff) ;
      WARNING("Error " << compare << " in " << *this);
      err = new patErrMiscError("Error with derivatives") ;
      WARNING(err->describe()) ;
      return NULL ;
    }
  }
#endif

  DELETE_PTR(theRowIter) ;
  DELETE_PTR(theMetaIter) ;
  return &result ;
}





void bioArithSum::first(patError*& err) {

  if (_literalIds == NULL) {
    // If the _literalIds has not been defined already, take it from
    // the repository.
    DEBUG_MESSAGE("Set betaIds") ;
    betaIds = bioLiteralRepository::the()->getBetaIds(patFALSE, err)  ;
    DEBUG_MESSAGE("betaIds size: " << betaIds.size()) ;
    _literalIds = &betaIds ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return ;
    }
    // Define also the spans
    // bioIteratorSpan theDefaultSpan("defaultSpan",0) ;
    // setCurrentSpan(theDefaultSpan);
    // setThreadSpan(theDefaultSpan) ;

  }
  scaleDerivatives = (bioParameters::the()->getValueInt("scaleDerivativesInSums") != 0) ;
  
  theScale = 0.0 ;
  if (scaleDerivatives) {
    theScale = patMax(patOne,patAbs(getValue(patFALSE,patLapForceCompute,err))) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return  ;
    }
  }

  if (isTop() && bhhh == NULL) {
    trParameters p = bioParameters::the()->getTrParameters() ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return  ;
    }
    bhhh = new trHessian(p,_literalIds->size()) ;
  }
  
  if (result.empty()) {
    result.resize(_literalIds->size()) ;
  }
  result.theFunction = 0.0 ;
  
  gradient.resize(_literalIds->size()) ;
  fill(gradient.begin(),gradient.end(),0.0) ;
  if (_computeHessian) {
    result.theHessian.setToZero() ;
  }

  if(bhhh != NULL) {
    bhhh->setToZero() ;
  }
  nIter = 0 ;

  if (theIteratorType == ROW) {
    if (theSample == NULL) {
      err = new patErrNullPointer("bioSample") ;
      WARNING(err->describe()) ;
      return ;
    }
    theRowIter = theSample->createRowIterator(theCurrentSpan,
					      theThreadSpan,
					      patFALSE,
					      err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return ;
    }

    theRowIter->first() ;
  }
  else if (theIteratorType == META) {
    if (theSample == NULL) {
      err = new patErrNullPointer("bioSample") ;
      WARNING(err->describe()) ;
      return ;
    }
    theMetaIter = theSample->createMetaIterator(theCurrentSpan,theThreadSpan,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return ;
    }
    theMetaIter->first() ;
  }

  return ;
}

void bioArithSum::next(patError*& err) {
  ++nIter ;
  if (theIteratorType == ROW) {
    theRowIter->next() ;
  }
  else if (theIteratorType == META) {
    theMetaIter->next() ;
  }
}

patBoolean bioArithSum::isDone(patError*& err) {

  if (theIteratorType == ROW) {
    if (theRowIter->isDone()) {
      return patTRUE ;
    }
    else {
      return patFALSE ;
    }
  }
  else if (theIteratorType == META) {
    if (theMetaIter->isDone()) {
      return patTRUE ;
    }
    else {
      return patFALSE ;
    }
  }
  return patTRUE ;
}
bioFunctionAndDerivatives*  bioArithSum::currentItem(patError*& err) {

  if (theIteratorType == ROW) {
    theRow = theRowIter->getCurrentRow() ;
    child->setVariables(theRowIter->currentItem()) ;
    if (weight != NULL && isTop()) {
      weight->setVariables(theRowIter->currentItem()) ;
    }
  }
  else if (theIteratorType == META) {
    child->setCurrentSpan(theMetaIter->currentItem()) ;
    if (weight != NULL && isTop()) {
      weight->setCurrentSpan(theMetaIter->currentItem()) ;
    }
  }
  bioFunctionAndDerivatives* fg = child->getNumericalFunctionAndGradient(*_literalIds,_computeHessian,_debugDeriv,err) ;
  if (err != NULL) {
    stringstream str ;
    str << "Row " << theRow << ": " << err->describe() ; 
    DELETE_PTR(err) ;
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe()) ;
    return NULL ;
  }
  // Calculate the weight
  if (weight != NULL && isTop()) {
    theWeightValue = weight->getValue(patFALSE, patLapForceCompute, err) ;
    fg->weight(theWeightValue,err) ;
  }
  if (err != NULL) {
    stringstream str ;
    str << "Row " << theRow << ": " << err->describe() ; 
    DELETE_PTR(err) ;
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe()) ;
    return NULL ;
  }
  return fg ;
}

patIteratorErr<bioFunctionAndDerivatives*>* bioArithSum::getIteratorForStochasticGradient(patError*& err) {
  return this ;
}
