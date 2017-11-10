//-*-c++-*------------------------------------------------------------
//
// File name : bioOptimizationResults.cc
// Author :    Michel Bierlaire
// Date :      Wed Jun 28 19:11:10 2017
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "bioOptimizationResults.h"
#include "patSvd.h"
#include "bioParameters.h"
#include "patErrNullPointer.h"

bioOptimizationResults::bioOptimizationResults(patBoolean _secondDerivatives,
					       patULong n) :
solution(n),
  gradient(n),
  hessian(NULL),
  bhhh(NULL),
  size(n),
  secondDerivatives(_secondDerivatives),
  inverseComputed(patFALSE), 
  smallestSingularValue(0.0) {
  }

bioOptimizationResults::~bioOptimizationResults() {
  DELETE_PTR(hessian) ;
  DELETE_PTR(bhhh) ;
}

patBoolean bioOptimizationResults::compute(patError*& err) {
  if (inverseComputed) {
    return patTRUE ;
  }
  if (!secondDerivatives) {
    return patFALSE ;
  }
  if (hessian == NULL) {
    err = new patErrNullPointer("patMyMatrix") ;
    WARNING(err->describe()) ;
    return patFALSE ;
  }
  patMyMatrix theHessian = hessian->getMatrixForLinAlgPackage(err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return patFALSE ;
  }
  patSvd svd(&theHessian) ;
  patULong svdMaxIter = bioParameters::the()->getValueInt("svdMaxIter",err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return patFALSE ;
  }
  svd.computeSvd(svdMaxIter,err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return patFALSE ;
  }
  const patMyMatrix* sol = svd.computeInverse() ;

  if (sol == NULL) {
    DEBUG_MESSAGE("Unable to compute the SVD") ;
    return patFALSE ;
  }
  else {
    inverseHessian = *sol ;
  }

  patReal svdThreshold = bioParameters::the()->getValueReal("singularValueThreshold",err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return patFALSE ;
  }
  eigenVectors = svd.getEigenVectorsOfZeroEigenValues(svdThreshold) ;
  smallestSingularValue = svd.getSmallestSingularValue() ;

  inverseComputed = patTRUE ;
  return patTRUE ;
 
}

void bioOptimizationResults::prepareMemoryForMatrices(trParameters theTrParameters, patError*& err) {
  hessian = new trHessian(theTrParameters,size) ;
  bhhh = new trHessian(theTrParameters,size) ;
}



patBoolean bioOptimizationResults::readyForSecondDerivatives() const {
  return secondDerivatives ;
}

void bioOptimizationResults::cancelInverseCalculation() {
  inverseComputed = patFALSE ;
}

patMyMatrix bioOptimizationResults::getInverseHessian(patError*& err) {
  compute(err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return patMyMatrix() ;
  }
  return inverseHessian ;
}

patULong bioOptimizationResults::getSize() const {
  return size ;
}

patBoolean bioOptimizationResults::isSandwichAvailable() const {
  if (hessian == NULL) {
    return patFALSE ;
  }
  if (bhhh == NULL) {
    return patFALSE ;
  }
  return patTRUE ;
}

patReal bioOptimizationResults::getGradientNorm() {
  patReal gradientNorm(0.0) ;
  for (trVector::iterator i = gradient.begin() ;
       i != gradient.end() ;
       ++i) {
    gradientNorm += (*i) * (*i) ;
  }
  return(sqrt(gradientNorm)) ;

}

patReal bioOptimizationResults::getSmallestSingularValue(patError*& err) {
  compute(err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return patReal() ;
  }
  return smallestSingularValue ;
}
