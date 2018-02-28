//-*-c++-*------------------------------------------------------------
//
// File name : bioArithSum.h
// Author :    Michel Bierlaire and Mamy Fetiarison
// Date :      Tue Jun 16 12:41:39 2009
//
//--------------------------------------------------------------------

#ifndef bioArithSum_h
#define bioArithSum_h


#include "bioArithIterator.h"
#include "bioLiteral.h"
#include "bioIteratorInfo.h"
#include "patIteratorErr.h"

class bioRowIterator ;
class bioMetaIterator ;

/*!
Class implementing a node of the tree representing a sum expression
*/
class bioArithSum : public bioArithIterator, public patIteratorErr<bioFunctionAndDerivatives*>  {

public:
  
  bioArithSum(bioExpressionRepository* rep,
	      patULong par,
	      patULong left,
	      patString anIterator,
              patULong w,
	      patError*& err) ;

  ~bioArithSum() ;

public:
  virtual bioExpression* getDerivative(patULong aLiteralId, 
				       patError*& err) const ;

  virtual trHessian* getBhhh() ;
  virtual patBoolean isSumIterator() const ;
  virtual patBoolean isSum() const  ;
  virtual patBoolean isProd() const  ;
  virtual patString getOperatorName() const ;
  virtual bioArithSum* getDeepCopy(bioExpressionRepository* rep,
				   patError*& err) const ;
  virtual bioArithSum* getShallowCopy(bioExpressionRepository* rep,
				   patError*& err) const ;
  virtual patString getExpressionString() const ;
  virtual patReal getValue(patBoolean prepareGradient, patULong currentLap, patError*& err)   ;
  virtual patULong getNumberOfOperations() const ;

  virtual bioFunctionAndDerivatives* 
  getNumericalFunctionAndGradient(vector<patULong> literalIds, 
				  patBoolean computeHessian,
				  patBoolean debugDerivatives,
				  patError*& err) ;

  // This function should be implemented only for the bioArithSum
  // operator. It is designed ot be used by the stochastic gradient
  // algorithm. It returns the expression as an iterator, given the
  // algoroithm access to each individual function calculation.
  
  patIteratorErr<bioFunctionAndDerivatives*> *getIteratorForStochasticGradient(patError*& err) ;

  
public:
  void first(patError*& err) ;
  void next(patError*& err) ;
  patBoolean isDone(patError*& err) ;
  bioFunctionAndDerivatives*  currentItem(patError*& err) ;

private:


  vector<patULong> theCompositeLiteralsIds ;
  trHessian* bhhh ;
  // Used only for WESML.
  patULong weightId ;
  bioExpression* weight ;
  patULong accessToFirstRow ;

  patBoolean scaleDerivatives ;
  patReal theScale ;
  vector<patReal> gradient ;
  bioRowIterator* theRowIter ;
  bioMetaIterator* theMetaIter ;
  patULong nIter  ;
  patULong theRow ;
  patReal theWeightValue ;
  vector<patULong>* _literalIds;
  vector<patULong> betaIds;
  patBoolean _computeHessian ;
  patBoolean _debugDeriv ;

  
};

#endif
