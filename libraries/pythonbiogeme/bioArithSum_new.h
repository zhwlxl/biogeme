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
//#include "bioSample.h"
//#include "bioSampleIterator.h"
#include "bioIteratorInfo.h"


/*!
Class implementing a node of the tree representing a sum expression
*/
class bioArithSum : public bioArithIterator {

public:
  
  bioArithSum(bioExpression *par,
                           bioExpression *left,
                           bioIteratorInfo *anIterator) ;

  ~bioArithSum() ;

public:
  virtual isSum() const = PURE_VIRTUAL ;
  virtual isProd() const = PURE_VIRTUAL ;

};

#endif
