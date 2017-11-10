//-*-c++-*------------------------------------------------------------
//
// File name : bioComputeIntegral.cc
// Author :    Michel Bierlaire
// Date :      
//
//--------------------------------------------------------------------

#include "bioComputeIntegral.h"
#include "bioMori.h"
#include "patErrMiscError.h"
#include "patDisplay.h"

bioComputeIntegral::bioComputeIntegral(bioMoriFunction* f) :
  lowerInfinity(patTRUE), 
  lowerBound(-patMaxReal),
  upperInfinity(patTRUE), 
  upperBound(patMaxReal), 
  theFunction(f)  {

}

bioComputeIntegral::bioComputeIntegral(bioMoriFunction* f,
				       patReal l, 
				       patReal u) :
  lowerInfinity(patFALSE), 
  lowerBound(l),
  upperInfinity(patFALSE), 
  upperBound(u), 
  theFunction(f)  {

}

patReal bioComputeIntegral::compute(patError*& err) {
  if (lowerInfinity || upperInfinity) {
    err = new patErrMiscError("Not yer implemented") ;
    WARNING(err->describe()) ;
    return patReal() ;
  }
  patReal result = DEIntegrator<bioMoriFunction>::Integrate(*theFunction,
							    lowerBound,
							    upperBound,
							    1.0e-6,
							    err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return patReal() ;
  }
  return result ;
}

