//----------------------------------------------------------------
// File: patRandomWalk.h
// Author: Michel Bierlaire
// Creation: Tue May 19 17:09:55 2009
//----------------------------------------------------------------

#ifndef patRandomWalk_h
#define patRandomWalk_h

#include "patPathGenerationAlgo.h"
#include "patUnixUniform.h"

class patRandomWalk : public patPathGenerationAlgo {

 public:
  patRandomWalk(patReal a, patReal b, patULong seed = 0) ;
  pair<patPath,patReal> generateNextPath(patNetwork* aNetwork, patOd aOd,
					 patError*& err) ;
  // Computes the probability that the path would have been generated
  // by the random walk algorithm
  patReal probability(patPath aPath, patNetwork* aNetwork, patError*& err) ;
  patString getReport() ;
  void setKumaParameters(patReal a, patReal b) ;  
 private:

  patUnixUniform randomNumbersGenerator ;
  pair<patULong,patReal> getNextNodeInRandomWalk(patNetwork* aNetwork, 
						 patULong currentNode, 
						 patULong dest, 
						 patError*& err) ;
  patReal kumaA ;
  patReal kumaB ;
};

#endif
