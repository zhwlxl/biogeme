//-*-c++-*------------------------------------------------------------
//
// File name : bioMhCombinedTransition.cc
// Author :    Michel Bierlaire
// Date :      Fri Aug 13 09:45:37 2010
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "bioMhCombinedTransition.h"
#include "patErrNullPointer.h"
#include "patErrOutOfRange.h"
#include "patUniform.h"

bioMhCombinedTransition::bioMhCombinedTransition(bioMarkovProcess* first, 
						 bioMarkovProcess* second, 
						 patReal probaFirst,
						 patUniform* rng) :
  theFirstProcess(first),
  theSecondProcess(second),
  probaFirstTransition(probaFirst),
  theRng(rng) {
  
}

bioMarkovTransition bioMhCombinedTransition::getNextState(bioMarkovState* cs,
							  patError*& err) {
  if (theFirstProcess == NULL) {
    err = new patErrNullPointer("bioMarkovProcess") ;
    WARNING(err->describe());
    return bioMarkovTransition() ;
  }
  if (theSecondProcess == NULL) {
    err = new patErrNullPointer("bioMarkovProcess") ;
    WARNING(err->describe());
    return bioMarkovTransition() ;
  }
  if (theRng == NULL) {
    err = new patErrNullPointer("patUniform") ;
    WARNING(err->describe());
    return bioMarkovTransition() ;
  }
  if ((probaFirstTransition < 0.0) || (probaFirstTransition > 1.0)) {
    err = new patErrOutOfRange<patReal>(probaFirstTransition,0.0,1.0) ;
    WARNING(err->describe()) ;
    return bioMarkovTransition() ;
  }

  patReal draw = theRng->getUniform(err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return bioMarkovTransition() ;    
  }


  bioMarkovTransition result ;
  if (draw <= probaFirstTransition) {
    result = theFirstProcess->getNextState(cs,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return bioMarkovTransition() ;    
    }
    result.theProbaForward *= probaFirstTransition ;
    result.theProbaBackward *= probaFirstTransition ;
    result.name = "Combined" ;
    ofstream f("states.log",ios::app) ;
    f << result.print() << endl ;
    f.close() ;
    
    return result ;
  }
  else {
    result = theSecondProcess->getNextState(cs,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return bioMarkovTransition() ;    
    }
    result.theProbaForward *= (1.0 - probaFirstTransition) ;
    result.theProbaBackward *= (1.0 - probaFirstTransition) ;
    result.name = "Combined" ;
    ofstream f("states.log",ios::app) ;
    f << result.print() << endl ;
    f.close() ;

    return result ;
  }
}

