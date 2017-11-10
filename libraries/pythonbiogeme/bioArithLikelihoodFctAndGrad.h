//-*-c++-*------------------------------------------------------------
//
// File name : bioArithLikelihoodFctAndGrad.h
// Author :    Michel Bierlaire
// Date :      Thu Apr 28 08:22:36 2011
//
//--------------------------------------------------------------------

#ifndef bioArithLikelihoodFctAndGrad_h
#define bioArithLikelihoodFctAndGrad_h

#include "patError.h"


class bioArithSum ;
/*!  Class implementing an iterator on a list of expressions.  Note
that, mathematically, a list of sums is equivalent to a sum of lists.
But this class is designed to keep the structure of the sum, necessary
to compute the BHHH, which requires the gradient for each element of
the sum. This is typically required for a loglikelihood function, so the name. 

 */
class bioArithLikelihoodFctAndGrad : public bioArithListOfExpressions {
public:

  bioArithLikelihoodFctAndGrad(patULong par,
			       patString aIter,
			       vector<patULong> fg, 
			       patError*& err) ;

  virtual vector<patReal> getValues(patError*& err) const ;
  
  virtual patString getOperatorName() const;
  virtual bioExpression* getDerivative(patULong aLiteralId, patError*& err) const ;
  virtual bioArithLikelihoodFctAndGrad* getDeepCopy(bioExpressionRepository* rep,patError*& err) const ;
  virtual patString getExpression(patError*& err) const ;
  virtual patULong getNumberOfOperations() const ;
  virtual patString theIterator() const ;

  virtual patString getExpressionString() const ;
  patBoolean containsAnIterator() const ;

private:
  patString theIteratorName ;
  bioIteratorType theIteratorType ;
};
#endif
