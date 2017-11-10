//-*-c++-*------------------------------------------------------------
//
// File name : trBasicTrustRegionAlgo.cc
// Author :    Michel Bierlaire
// Date :      Thu Jan 20 14:05:22 2000
//
// Implementation of algorithm 6.1.1: the Basic Trust-Region Algorithm
//
// Source: Conn, Gould Toint (2000) Trust Region Methods
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


// For debug only
#include <numeric>
//

#include <iomanip>
#include "patConst.h"
#include "patMath.h"
#include "patNonLinearProblem.h"
#include "patErrNullPointer.h"
#include "patErrMiscError.h"
#include "trBasicTrustRegionAlgo.h"
#include "trPrecond.h"
#include "trFunction.h"
#include "trHessian.h"
#include "trBFGS.h"
#include "trTointSteihaug.h"

trBasicTrustRegionAlgo::trBasicTrustRegionAlgo(patNonLinearProblem* aProblem,
					       trParameters p,
					       patError*& err) :
   f(NULL), 
   status(trUNKNOWN), 
   hessian(NULL), 
   quasiNewtonBFGS(NULL), 
   precond(NULL),
   iter(0) {
   if (aProblem == NULL) {
     err = new patErrNullPointer("patNonLinearProblem")  ;
     WARNING(err->describe()) ;
     return ;
   }
   f = aProblem->getObjective(err) ;
   if (err != NULL) {
     WARNING(err->describe()) ;
     return ;
   }
   exactHessian = 
     f->isHessianAvailable() && 
     (theParameters.exactHessian != 0);
   
   if (exactHessian) {
     hessian = f->getHessian() ;
   }
}


trBasicTrustRegionAlgo::trBasicTrustRegionAlgo(trFunction* _f,
					       const trVector& initSolution,
					       trParameters p,
					       patError*& err) :
  f(_f),
  status(trUNKNOWN),
  solution(initSolution),
  gk(initSolution.size()),
  theParameters(p),
  radius(p.initRadius),
  quasiNewtonBFGS(NULL),
  precond(NULL) {

  if (f == NULL) {
    err = new patErrNullPointer("trFunction") ;
    WARNING(err->describe()) ;
    return ;
  }
  
  exactHessian = f->isHessianAvailable() && (theParameters.exactHessian != 0);
  
  if (exactHessian) {
    hessian = f->getHessian() ;
  }
}

trBasicTrustRegionAlgo::~trBasicTrustRegionAlgo() {
  if (quasiNewtonBFGS != NULL) {
    DELETE_PTR(quasiNewtonBFGS) ;
  }
  if (precond != NULL) {
    DELETE_PTR(precond) ;
  }
}

trBasicTrustRegionAlgo::trTermStatus trBasicTrustRegionAlgo::getTermStatus() {
  return status ;
}

trVector trBasicTrustRegionAlgo::getSolution(patError*& err) {
  if (err != NULL) {
    WARNING(err->describe()) ;
    return trVector();
  }
  return(solution) ;
}

patReal trBasicTrustRegionAlgo::getValueSolution(patError*& err) {
  if (err != NULL) {
    WARNING(err->describe()) ;
    return patReal() ;
  }
  if (f == NULL) {
    err = new patErrNullPointer("patFunction") ;
    WARNING(err->describe()) ;
    return patReal() ;
  }
  patBoolean success ;
  patReal result = f->computeFunction(&solution,&success,err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return patReal() ;
  }
  if (!success) {
    WARNING("**** Numerical problem in function evaluation") ;
  }
  return result ;
}

patString trBasicTrustRegionAlgo::run(patError*& err) {
  if (err != NULL) {
    WARNING(err->describe()) ;
    return patString("UNKNOWN");
  }

  // Compute the function, gradient and hessian at the initial solution

  if (f == NULL) {
    err = new patErrNullPointer("trFunction") ;
    WARNING(err->describe()) ;
    return patString("UNKNOWN") ;
  }

  patBoolean success ;

  function = f->computeFunctionAndDerivatives(&solution,exactHessian,&success,err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return patString("UNKNOWN");
  }
  if (!success) {
    err = new patErrMiscError("Numerical problem during function computation at the starting point") ;
    WARNING(err->describe()) ;
    return patString("UNKNOWN") ;
  }

  if (!theParameters.exactHessian) {
    if (theParameters.initQuasiNewtonWithTrueHessian) {
      patHybridMatrix* ptr = f->getHessian()->getHybridMatrixPtr() ;
      if (ptr == NULL) {
	err = new patErrNullPointer("patHybridMatrix") ;
	WARNING(err->describe()) ;
	return patString("UNKNOWN");
      }
      hessian = quasiNewtonBFGS = new trBFGS(*ptr,
				   theParameters,
				   err) ;
      if (err != NULL) {
	WARNING(err->describe()) ;
	return patString("UNKNOWN");
      }
    }
    else {
      hessian = quasiNewtonBFGS = new trBFGS(solution.size(),
				   theParameters,
				   err) ;
      if (err != NULL) {
	WARNING(err->describe()) ;
	return patString("UNKNOWN");
      }
    }
    if (quasiNewtonBFGS == NULL) {
      err = new patErrNullPointer("trBFGS") ;
      WARNING(err->describe()) ;
      return patString("UNKNOWN") ;
    }
    DEBUG_MESSAGE("BFGS used") ;
  }
  else {
    hessian = f->getHessian() ;
  }

  // Check if the hessian can be used  for preconditionning

  if (precond != NULL) {
    DELETE_PTR(precond) ;
  }
  if (hessian->providesPreconditionner() && theParameters.usePreconditioner) {
    precond = hessian->createPreconditionner(err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return patString("UNKNOWN") ;
    }
  }
  else {
    precond = new trPrecond() ;
  }

  
  iter = 0 ;
  GENERAL_MESSAGE("    gmax Iter   radius   Status        f(cand)      rhok ") ; 

  while (!stop(err) && iter < theParameters.maxIter) {
    ++iter ;
    cout << setw(4) << iter << " " << setfill(' ') << setiosflags(ios::scientific) << setprecision(2) << radius << " " ;

    //    DEBUG_MESSAGE("Call Toint-Steihaug with ");
    //   cout << *(trHessian*)hessian << endl ;
    trTointSteihaug model(&gk,hessian,radius,precond,theParameters,err) ;
    
    if (err != NULL) {
      WARNING(err->describe()) ;
      return patString("UNKNOWN") ;
    }
    
    trTointSteihaug::trTermStatus status = model.run(err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return patString("UNKNOWN") ;
    }

    cout <<  setiosflags(ios::right)  <<setw(12) <<  setfill('*') 
	 <<  model.getStatusName(status) << setw(1) << " "  ;
    trVector candidate = solution + model.getSolution(err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return patString("UNKNOWN") ;
    }

    patReal valCandidate = f->computeFunction(&candidate,&success,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return patString("UNKNOWN") ;
    }
    if (!success) {
      valCandidate = patMaxReal ;
    }

    cout <<  setfill(' ') << setiosflags(ios::scientific|ios::showpos) << setprecision(7) 
	 << valCandidate << " " ; 

    patReal rhok = (valCandidate - function) / model.getValue(err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return patString("UNKNOWN") ;
    }

    cout << setfill(' ') << setiosflags(ios::scientific|ios::showpos) << setprecision(2)
	 << rhok << " " ;
    
    cout << resetiosflags(ios::showpos) ;
    trVector oldGk ;
    if (rhok >= theParameters.eta1) {
      cout << " +" ;
      // Successful iteration
      solution = candidate ;
      function = valCandidate ;
      // ***************************
      // Compute here gradient and hessian of the new iterate
      //****************************
      
      oldGk = gk ;
      f->computeFunctionAndDerivatives(&solution,exactHessian,&success,err) ;
      if (err != NULL) {
	WARNING(err->describe()) ;
	return patString("UNKNOWN");
      }
      if (!success) {
	rhok = -1.0 ;
      }
    }

    if (rhok >= theParameters.eta1) {
      
      if (!exactHessian) {
	// Quasi-Newton update
	if (quasiNewtonBFGS == NULL) {
	  err = new patErrNullPointer("trBFGS") ;
	  WARNING(err->describe()) ;
	  return patString("UNKNOWN") ;
	}
	trVector sk = model.getSolution(err) ;
	quasiNewtonBFGS->update(sk,gk,oldGk,cout,err) ;
	if (err != NULL) {
	  WARNING(err->describe()) ;
	  return patString("UNKNOWN");
	}
      }


      // Check if the hessian can be used  for preconditionning
      
      if (precond != NULL) {
	DELETE_PTR(precond) ;
      }
      if (hessian->providesPreconditionner() && theParameters.usePreconditioner) {
	precond = hessian->createPreconditionner(err) ;
	if (err != NULL) {
	  WARNING(err->describe()) ;
	  return patString("UNKNOWN") ;
	}
      }
      else {
	precond = new trPrecond() ;
      }

      // Update trust region
      if (rhok >= theParameters.eta2) {
      cout << "+" ;
	// Very successful iteration
	radius *= theParameters.beta ; 
      }
    }
    else {
      cout << " - ";
      // Unsuccessful iteration
      radius = model.getNormStep() *  theParameters.gamma2 ;
    }

    if (radius > theParameters.maxTrustRegionRadius) {
      radius = theParameters.maxTrustRegionRadius ;
    }
    //    DEBUG_MESSAGE("f=\t" << function << "\t||g||=\t" 
    //		  << sqrt(inner_product(gk.begin(),gk.end(),gk.begin(),0.0))) ;
    //DEBUG_MESSAGE("tr=\t" << radius) ;
    cout << endl ;
  }

  cout << setprecision(7) << endl  ;

  if (iter == theParameters.maxIter) {
    DEBUG_MESSAGE("Maximum number of iterations reached") ;
    return patString("Maximum number of iterations reached") ; ;
  }
  else {
    DEBUG_MESSAGE("Convergence reached...") ;
    return patString("Convergence reached...") ;
  }
return patString("UNKNOWN") ;
}


patBoolean trBasicTrustRegionAlgo::stop(patError*& err) {
  patReal gMax = 0.0 ;
  //DEBUG_MESSAGE("gk="<<gk) ;

  trVector::iterator gIter = gk.begin() ;
  trVector::iterator xIter = solution.begin() ;
  for ( ; gIter != gk.end() ; ++gIter, ++xIter) {
    patReal gRel = patAbs(*gIter) * patMax(patOne,*xIter) / 
      patMax(patAbs(function),theParameters.typicalF) ;
    gMax = patMax(gMax,gRel) ;
  }
  cout << setfill(' ') << setiosflags(ios::scientific) << setprecision(2) 
 << gMax << " " ;
  //DEBUG_MESSAGE("gMax = " << gMax) ;
  return (gMax < theParameters.tolerance) ;

}

void trBasicTrustRegionAlgo::defineStartingPoint(const patVariables& x0) {
  solution = x0 ;
}

patVariables trBasicTrustRegionAlgo::getLowerBoundsLambda() {
  WARNING("Not yet implemented") ;
  return patVariables(solution.size(),0.0) ;
}

patVariables trBasicTrustRegionAlgo::getUpperBoundsLambda() {
  WARNING("Not yet implemented") ;
  return patVariables(solution.size(),0.0) ;
}

patString trBasicTrustRegionAlgo::getName() {
  return patString("BTRA") ;
}

patULong trBasicTrustRegionAlgo::nbrIter() {
  return iter ;
}
