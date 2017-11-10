//-*-c++-*------------------------------------------------------------
//
// File name : trBasicTrustRegionAlgo.h
// Date :      Thu Jan 20 09:14:35 2000
//
//--------------------------------------------------------------------


#ifndef trBasicTrustRegionAlgo_h
#define trBasicTrustRegionAlgo_h

#include "patError.h"
#include "trVector.h"
#include "trNonLinearAlgo.h"
#include "trParameters.h"

class trFunction ;
class trMatrixVector;
class trPrecond ;

class trHessian ;
class trBFGS ;

class patNonLinearProblem ;
/** 
   @doc Implementation of algorithm 6.1.1: the Basic Trust-Region Algorithm.
   @see Conn, Gould Toint (2000) Trust Region Methods, SIAM
   @author \URL[Michel Bierlaire]{http://rosowww.epfl.ch/mbi}, EPFL (Thu Jan 20 09:14:35 2000)
*/

class trBasicTrustRegionAlgo : public trNonLinearAlgo {
public :  

  /**
   */
  enum trTermStatus {
    /**
     */
    trUNKNOWN , 
    /**
     */
    trMAXITER, 
    /**
     */
    trCONV}   ;

  /**
   */
  trBasicTrustRegionAlgo(patNonLinearProblem* aProblem, 
			 trParameters theParam,
			 patError*& err) ;

  /**
     @param f pointer to the function to minimize
     @param initSolution initial solution
     @param _radius initial radius of the trust region
     @param err ref. of the pointer to the error object.
   */
  trBasicTrustRegionAlgo(trFunction* f,
			 const trVector& initSolution,
			 trParameters theParam,
			 patError*& err) ;
  /**
   */
  virtual ~trBasicTrustRegionAlgo() ;

  /**
     Executes the algorithm
     @param err ref. of the pointer to the error object.
     @return termination status
   */
  patString run(patError*& err) ;
  /**
     @return number of iterations. If there is any error, 0 is returned. 
   */
  patULong nbrIter() ;
  /**
   */
  trTermStatus getTermStatus() ;
  /**
   */
  patVariables getSolution(patError*& err) ;
  /**
   */
  patReal getValueSolution(patError*& err) ;

  /**
   */
  void defineStartingPoint(const patVariables& x0) ;
  /**
   */
  patVariables getLowerBoundsLambda() ;
  /**
   */
  patVariables getUpperBoundsLambda()  ;
  
  /**
   */
  virtual patString getName() ;
private:
  patBoolean stop(patError*& err) ;
private :
  trFunction* f ;
  trTermStatus status ;
  trVector solution ;
  patReal function ;
  trVector gk ;
  patReal radius ;
  // Generic hessian, unaware of its real nature
  trMatrixVector* hessian ;
  // Specific hessian. Usually, only one will be non zero.  The generic
  // pointer is used to avoid to check which one is non zero when no specific
  // function is requoired.
  //trHessian* trueHessian ;
  trBFGS* quasiNewtonBFGS ; 
  trPrecond* precond ;
  int iter ;
  trParameters theParameters ;
  patBoolean exactHessian ;

};


#endif
