//-*-c++-*------------------------------------------------------------
//
// File name : bioArithListOfExpressions.h
// Author :    Michel Bierlaire
// Date :      Wed Apr 13 15:57:03 2011
//
//--------------------------------------------------------------------

#ifndef bioArithListOfExpressions_h
#define bioArithListOfExpressions_h

#include <vector>
#include "patType.h"
#include "patError.h"
#include "bioArithMultinaryExpression.h"

class bioArithNamedExpression ;
class bioSample ;
class trHessian ;

class bioArithListOfExpressions : public bioArithMultinaryExpression {

public:
  bioArithListOfExpressions(patULong par,vector<patULong> l) ;
  trHessian* getBhhh() ;
			       
  virtual vector<patReal> getValues(patError*& err) const ;
  virtual patString getOperatorName() const  ;
  virtual patReal getValue(patError*& err) const  ;
  virtual bioExpression* getDerivative(patULong aLiteralId, 
				       patError*& err) const ;
  virtual bioArithListOfExpressions* getDeepCopy(bioExpressionRepository* rep, 
						 patError*& err) const ;
  virtual bioArithListOfExpressions* getShallowCopy(bioExpressionRepository* rep, 
						 patError*& err) const ;
  virtual patString getExpressionString() const ;
  virtual bioArithListOfExpressions* getFunctionAndDerivatives(vector<patULong> literalIds, 
							      patError*& err) const ;

protected:
  trHessian* bhhh ;


};


#endif
