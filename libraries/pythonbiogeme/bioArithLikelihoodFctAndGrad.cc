//-*-c++-*------------------------------------------------------------
//
// File name : bioArithLikelihoodFctAndGrad.cc
// Author :    Michel Bierlaire
// Date :      Thu Apr 28 08:37:02 2011
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "bioArithLikelihoodFctAndGrad.h"
#include "patErrMiscError.h"
#include "patErrNullPointer.h"
#include "patDisplay.h"
#include "bioParameters.h"
#include "trHessian.h"
#include "bioArithSum.h"
#include "bioRowIterator.h"
#include "bioMetaIterator.h"
#include "bioSample.h"
#include "bioIteratorInfoRepository.h"
#include "bioExpressionRepository.h"

bioArithLikelihoodFctAndGrad::
bioArithLikelihoodFctAndGrad(patULong par,
			     patString aIter,
			     vector<patULong> fg,
			     patError*& err)
  : bioArithListOfExpressions(par,fg),
    theIteratorName(aIter) {

  // It is assumed that the size of the list is n+1, where n is the
  // size of the gradient vector. So, bhhh must be n times n
  trParameters p = bioParameters::the()->getTrParameters(err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return ;
  }
  bhhh = new trHessian(p,listOfChildren.size()-1) ;

  bioIteratorSpan theSpan(theIteratorName,0) ;
  setCurrentSpan(theSpan) ;
  theIteratorType = bioIteratorInfoRepository::the()->getType(theIteratorName,err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return ;
  }

}


patString bioArithLikelihoodFctAndGrad::getOperatorName() const {
  return patString("Loglikelihood") ;
}

vector<patReal> bioArithLikelihoodFctAndGrad::getValues(patError*& err) const {

  if (theSample == NULL) {
    err = new patErrNullPointer("bioSample") ;
    WARNING(err->describe()) ;
    return vector<patReal>() ;
  }

  vector<patReal> result(listOfChildren.size(),0.0) ;
  if (bhhh != NULL) {
    bhhh->setToZero() ;
  }
  if (theIteratorType == ROW) {
    bioRowIterator* theIter = theSample->createRowIterator(theCurrentSpan,theThreadSpan,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return vector<patReal>() ;
    }
    
    for (theIter->first() ;
	 !theIter->isDone() ;
	 theIter->next()) {
      for (vector<bioExpression*>::const_iterator i = listOfChildren.begin() ;
	   i != listOfChildren.end() ;
	   ++i) {
	(*i)->setVariables(theIter->currentItem()) ;
      }
      vector<patReal> iterResult = bioArithListOfExpressions::getValues(err) ;
      if (err != NULL) {
	WARNING(err->describe()) ;
	return vector<patReal>() ;
      }
      
      result += iterResult ;
      for (patULong i = 1 ; i < listOfChildren.size() ; ++i) {
	for (patULong j = i ; j < listOfChildren.size() ; ++j) {
	  bhhh->addElement(i-1,j-1,iterResult[i]*iterResult[j],err) ;
	  if (err != NULL) {
	    WARNING(err->describe()) ;
	    return vector<patReal>() ;
	  }
	}
      }
    }
    return result ;
  }
  else if (theIteratorType == META) {
    bioMetaIterator* theIter = theSample->createMetaIterator(theCurrentSpan,theThreadSpan,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return vector<patReal>() ;
    }
    for (theIter->first() ;
	 !theIter->isDone() ;
	 theIter->next()) {
      for (vector<bioExpression*>::const_iterator i = listOfChildren.begin() ;
	   i != listOfChildren.end() ;
	   ++i) {
	(*i)->setCurrentSpan(theIter->currentItem()) ;
      }
      vector<patReal> iterResult = bioArithListOfExpressions::getValues(err) ;
      if (err != NULL) {
	WARNING(err->describe()) ;
	return vector<patReal>() ;
      }
      result += iterResult ;
      for (patULong i = 1 ; i < listOfChildren.size() ; ++i) {
	for (patULong j = 1 ; j < listOfChildren.size() ; ++j) {
	  bhhh->addElement(i-1,j-1,iterResult[i]*iterResult[j],err) ;
	  if (err != NULL) {
	    WARNING(err->describe()) ;
	    return vector<patReal>() ;
	  }
	}
      }
    }
    return result ;
  }
  else if (theIteratorType == DRAW) {
    err = new patErrMiscError("Loglikelihood cannot be based on a draw iterator") ;
    WARNING(err->describe()) ;
    return vector<patReal>() ;

  }
}


bioExpression* bioArithLikelihoodFctAndGrad::getDerivative(patULong aLiteralId, patError*& err) const {
  err = new patErrMiscError("In principle, the derivative of this object should not be used. It is likely that it has been called by mistake.") ;
  WARNING(err->describe()) ;
  return NULL ;
  
}
bioArithLikelihoodFctAndGrad* bioArithLikelihoodFctAndGrad::getDeepCopy(patError*& err) const {

  vector<patULong> newListOfChildren ;
  for (vector<bioExpression*>::const_iterator i = listOfChildren.begin() ;
       i != listOfChildren.end() ;
       ++i) {
    bioExpression* n = (*i)->getDeepCopy(err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return NULL ;
    }
    newListOfChildren.push_back(n->getId()) ;
  }
  bioArithLikelihoodFctAndGrad* newObject = new bioArithLikelihoodFctAndGrad(patBadId,theIteratorName,newListOfChildren,err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return NULL ;
  }
  return newObject ;
}

patString bioArithLikelihoodFctAndGrad::getCppCode(patString prefix, patError*& err) {
  err = new patErrMiscError("Not implemented") ;
  WARNING(err->describe()) ;
  return patString() ;
}

patString bioArithLikelihoodFctAndGrad::getSimpleCppCode(patError*& err) {
  err = new patErrMiscError("Not implemented") ;
  WARNING(err->describe()) ;
  return patString() ;
  
}
patString bioArithLikelihoodFctAndGrad::getExpression(patError*& err) const {
  patString theInfo =  bioIteratorInfoRepository::the()->getInfo(theIteratorName,err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return patString() ;
  }
  
  patString theExpression =  bioArithListOfExpressions::getExpression(err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return patString() ;
  }

  stringstream ss ;
  ss << "Sum(" ;
  ss << theInfo << "," <<  theExpression << ")";
  return patString(ss.str()) ;
  
}




patULong bioArithLikelihoodFctAndGrad::getNumberOfOperations() const {
  patError* err(NULL) ;
  patULong result = bioArithListOfExpressions::getNumberOfOperations() ;
  if (theIteratorType == DRAW) {
    patULong nDraws = bioParameters::the()->getValueInt("NbrOfDraws",err) ;
    result *= nDraws ;
  }
  else {
    result *= bioIteratorInfoRepository::the()->getNumberOfIterations(theIteratorName,err) ;
  }
  return result ;
}

patString bioArithLikelihoodFctAndGrad::getExpressionString() const {
  stringstream str ;
  str << "$LL" ;
  str << theIteratorName ;
  str << "{" << bioArithListOfExpressions::getExpressionString() << "}" ;
  return patString(str.str()) ;
}

patBoolean bioArithLikelihoodFctAndGrad::containsAnIterator() const {
  return patTRUE ;
}

patString bioArithLikelihoodFctAndGrad::theIterator() const {
  return theIteratorName ;
}
