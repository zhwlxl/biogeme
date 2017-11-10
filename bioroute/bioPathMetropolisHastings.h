//----------------------------------------------------------------
// File: bioPathMetropolisHastings.h
// Author: Michel Bierlaire
// Creation: Wed Aug 11 17:05:07 2010
//----------------------------------------------------------------

#ifndef bioPathMetropolisHastings_h
#define bioPathMetropolisHastings_h

#include "patPathGenerationAlgo.h"
#include "bioMhDetourTransition.h"
#include "bioMhShuffleTransition.h"
#include "bioMhRandomWalkTransition.h"
#include "bioMhCombinedTransition.h"
#include "bioMhPathGenState.h"
#include "patUnixUniform.h"

class bioMetropolisHastings ;
class bioPathMetropolisHastings : public patPathGenerationAlgo {
 public:
  bioPathMetropolisHastings(patString name, 
			    patULong warmup,
			    patULong simulationInterval,
			    patReal a,
			    patReal b,
			    patReal scaleDetour,
			    patReal scaleImportanceSampling,
			    patULong seed) ;

  virtual pair<patPath,patReal> generateNextPath(patNetwork* aNetwork, 
						 patOd aOd, 
						 patError*& err)  ;
  virtual patString getReport() ;

 private:
  patBoolean initialized ;

  patReal kumaA ;
  patReal kumaB ;
  bioMetropolisHastings* theCoreAlgorithm ;
  bioMhPathGenState* currentState ;
  bioMhDetourTransition* theDetourProcess ;
  bioMhShuffleTransition* theShuffleProcess ;
  bioMhRandomWalkTransition* theRandomWalkProcess ;
  bioMhCombinedTransition* theCombinedProcess ;
  bioMhCombinedTransition* theFullProcess ;
  patUnixUniform rng ;
  patReal theScaleDetour ;
  patReal theScaleImportanceSampling ;
  patULong theWarmup ;
};

#endif
