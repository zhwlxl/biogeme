//-*-c++-*------------------------------------------------------------
//
// File name : bioDraftSG
// Author :    Michel Bierlaire
// Date :      Fri Dec 15 11:19:16 2017
//
//--------------------------------------------------------------------


// Quick and dirty implementation of the sotchastic gradient
// function. Does not work for panel data. 


#ifndef bioDraftSG_h
#define bioDraftSG_h

#include "bioIteratorSpan.h"
#include "patError.h"
#include "trVector.h"
#include "bioStochasticGradientFunction.h"

class bioExpression ;
class bioExpressionRepository ;
class bioFunctionAndDerivatives ;
class bioSample ;

class bioDraftSG : public bioStochasticGradientFunction {

public :

  bioDraftSG(bioExpressionRepository* rep,
	     patULong expId,
	     bioSample* sample,
	     patError*& err) ;
  ~bioDraftSG() ;
  void init() ;
  void next() ;
  patBoolean isDone() ;
  bioFunctionAndDerivatives* getFunctionAndDerivatives(trVector* x,
						       patError*& err) ;
  void random() ;
  patULong getIndex() ;
  patULong getSize() ;
  
private:
  patULong theExpressionId ;
  bioExpression* theExpression ;
  bioSample* theSample ;
  bioIteratorSpan theSpan ;
  vector<patULong> betaIds ;
  patULong sampleSize ;
  patBoolean debugDeriv ;
  
};



#endif
