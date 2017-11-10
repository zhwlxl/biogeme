//----------------------------------------------------------------
// File: patMonteCarlo.cc
// Author: Michel Bierlaire
// Creation: Wed May 20 11:40:21 2009
//----------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "patMonteCarlo.h"
#include "patNetwork.h"
#include "patDisplay.h"
#include "patErrMiscError.h"

patMonteCarlo::patMonteCarlo(patULong seed) : 
  patPathGenerationAlgo(patString("Monte Carlo")), 
  stderr(0.0), 
  arng(seed) {

  theNormal.setUniform(&arng) ;
}

pair<patPath,patReal> patMonteCarlo::generateNextPath(patNetwork* theNetwork, patOd aOd, patError*& err) {
  theNetwork->generateMonteCarloCosts(&theNormal, stderr, patTRUE, err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return pair<patPath,patReal>() ;
  }
  patBoolean success ;
  patPath theShortest = theNetwork->getShortestPath(aOd,&success,err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return pair<patPath,patReal>() ;
  }


  theNetwork->restoreOriginalCosts(err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return pair<patPath,patReal>();
  }

  return pair<patPath,patReal>(theShortest,0.0) ;

}

void patMonteCarlo::setStandardError(patReal s) {
  stderr = s ;
}


patString patMonteCarlo::getReport() {

}
