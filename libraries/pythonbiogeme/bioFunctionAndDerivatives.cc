//-*-c++-*------------------------------------------------------------
//
// File name : bioFunctionAndDerivatives.cc
// Author :    Michel Bierlaire
// Date :      Sun May 22 09:55:57 2011
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "patMath.h"
#include "patDisplay.h"
#include "bioFunctionAndDerivatives.h"
#include "bioParameters.h"

bioFunctionAndDerivatives::bioFunctionAndDerivatives(patULong n) : theGradient(n,0.0), theHessian(bioParameters::the()->getTrParameters(),n) {
}

bioFunctionAndDerivatives::~bioFunctionAndDerivatives() {
}

ostream& operator<<(ostream &str, const bioFunctionAndDerivatives& x) {
  str << "[f]:" << x.theFunction ;
  for (patULong i = 0 ; i < x.theGradient.size() ; ++i) {
    str << " [g" << i << "]:" << x.theGradient[i] ;
  }
  str << x.theHessian ;
  return str ;
}

patBoolean bioFunctionAndDerivatives::empty() const {
  return theGradient.empty() ;
}
void bioFunctionAndDerivatives::resize(patULong s) {
  theGradient.resize(s) ;
  theHessian.resize(s) ;

}

void bioFunctionAndDerivatives::resize(patULong s, patReal r) {
  theGradient.resize(s,r) ;
  theHessian.resize(s) ;

}

patString bioFunctionAndDerivatives::printSize() {
  stringstream str ;
  str << "Gradient: " << theGradient.size() << ". " ;
  str << "Hessian: " << theHessian.getDimension() ;
  return patString(str.str()) ;
}

bioFunctionAndDerivatives& bioFunctionAndDerivatives::operator=(bioFunctionAndDerivatives& obj) {
  theFunction = obj.theFunction ;
  theGradient = obj.theGradient ;
  theHessian = obj.theHessian ;
  return *this ;
}

patReal bioFunctionAndDerivatives::compare(const bioFunctionAndDerivatives& x,
					   patError*& err) {

  if (theGradient.size() != x.theGradient.size()) {
    stringstream str ;
    str << "Incompatible sizes: " << theGradient.size() << " and " << x.theGradient.size() ;
    err = new patErrMiscError(str.str());
    WARNING(err->describe()) ;
    return patReal() ;
  }

  
  patULong  n1 = theHessian.getDimension() ;
  patULong  n2 = x.theHessian.getDimension() ;
  if (n1 != n2) {
    stringstream str ;
    str << "incompatible dimensions: " << n1 << " and " << n2 ;
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe()) ;
    return patReal() ;
  }

  
  patReal diff = patAbs(theFunction-x.theFunction) ;
  patReal maximum(diff) ;

  patString whereMax("function") ;
  for (patULong i = 0 ; i < theGradient.size() ; ++i) {
    diff = patAbs(theGradient[i] - x.theGradient[i]) ;
    if (diff > maximum) {
      maximum = diff ;
      whereMax = "gradient" ;
    }
  }

  patReal tolerance = bioParameters::the()->getValueReal("toleranceCheckDerivatives",err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return patReal() ;
  }

  if (maximum >= tolerance) {
    DEBUG_MESSAGE("Maximum " << maximum << " above " << tolerance << " reached for " << whereMax) ;
  }
  return maximum ;
}
