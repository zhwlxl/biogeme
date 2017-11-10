//-*-c++-*------------------------------------------------------------
//
// File name : bioArithListOfExpressions.cc
// Author :    Michel Bierlaire
// Date :      Tue Apr 19 10:41:47 2011
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "patDisplay.h"
#include "bioArithListOfExpressions.h"
#include "patErrMiscError.h"
#include "bioExpressionRepository.h"
bioArithListOfExpressions::bioArithListOfExpressions(patULong par,
						     vector<patULong> l) : bioArithMultinaryExpression(par,l),bhhh(NULL) {

}

vector<patReal> bioArithListOfExpressions::getValues(patError*& err) const{
  vector<patReal> result ;
  for (vector<bioExpression*>::const_iterator i = listOfChildren.begin() ;
       i != listOfChildren.end() ;
       ++i) {
    patReal r = (*i)->getValue(err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return vector<patReal>() ;
    }
    result.push_back(r) ;
  }
  return result ;
}


patString bioArithListOfExpressions::getOperatorName() const {
  return patString("List");
}

patReal bioArithListOfExpressions::getValue(patError*& err) const {
  err = new patErrMiscError("This expression has several values. This function should not be called.") ;
  WARNING(err->describe()) ;
  return patReal() ;
}

bioExpression* bioArithListOfExpressions::getDerivative(patULong aLiteralId, 
							patError*& err) const {


  vector<patULong> result ;
  for (vector<bioExpression*>::const_iterator i = listOfChildren.begin() ;
       i != listOfChildren.end() ;
       ++i) {
    bioExpression* r = (*i)->getDerivative(aLiteralId,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return NULL ;
    }
    result.push_back(r->getId()) ;
  }
  bioArithListOfExpressions* deriv = new bioArithListOfExpressions(patBadId,result) ;
  return deriv ;
}

bioArithListOfExpressions* bioArithListOfExpressions::getDeepCopy(patError*& err) const {

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
  bioArithListOfExpressions* newExpr = new bioArithListOfExpressions(patBadId,newListOfChildren) ;

  return newExpr ;
}

patString bioArithListOfExpressions::getCppCode(patString prefix, patError*& err) {
  err = new patErrMiscError("Not implemented");
  WARNING(err->describe()) ;
  return patString() ;
}

patString bioArithListOfExpressions::getSimpleCppCode(patError*& err) {
  err = new patErrMiscError("Not implemented");
  WARNING(err->describe()) ;
  return patString() ;

}

patString bioArithListOfExpressions::getExpressionString() const {
  stringstream str ;
  str << "//" ;
  for (vector<bioExpression*>::const_iterator i = listOfChildren.begin() ;
       i != listOfChildren.end() ;
       ++i) {
    str << "{"<< (*i)->getExpressionString() << "}" ;
  }
  str << "//" ;
  return patString(str.str());
}

trHessian* bioArithListOfExpressions::getBhhh() {
  return bhhh ;
}

bioArithListOfExpressions* bioArithListOfExpressions::getFunctionAndDerivatives(vector<patULong> literalIds, 
										patError*& err) const {
  err = new patErrMiscError("Function should not be called") ;
  WARNING(err->describe()) ;
  return NULL ;
}
