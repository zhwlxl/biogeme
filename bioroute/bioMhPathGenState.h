//-*-c++-*------------------------------------------------------------
//
// File name : bioMhPathGenState.h
// Author :    Michel Bierlaire
// Date :      Mon Aug  9 16:19:25 2010
//
//--------------------------------------------------------------------

#ifndef bioMhPathGenState_h
#define bioMhPathGenState_h

class patNode ;
#include "bioMarkovState.h"
#include "patType.h"
#include "patPath.h"
#include "patDisplay.h"

class patNetwork ;
/**

                             c 

Orig --> x --->  u ---> x ---> x ---> v ---> x ---> Dest


A state is a combination of 
- a path p
- two different nodes on the paths: u and v such that u is upstream of v
- a node c on the path, between u and v

The weight of a state for the MH algorithm is computed as follows:

exp( scale * (shortestPathCost - pathCost)) / #state with the same path.

With a scale of 0, the MH algo will generate a draw from a uniform
distribution among all possible paths.
With a scale close to infinity, it will generate only the shortest path.
*/

class bioMhPathGenState : public bioMarkovState {

  friend ostream& operator<<(ostream &str, const bioMhPathGenState& x) ;
  friend patBoolean operator<(const bioMhPathGenState& s1, const bioMhPathGenState& s2) ;

  friend class bioMhShuffleTransition ;
 public:
  bioMhPathGenState(patNetwork* n, 
		    patPath p, 
		    patULong u, 
		    patULong v, 
		    patNode* c,
		    patReal scaleDetour,
		    patReal scaleImportanceSampling,
		    patError*& err) ;
  virtual patReal getWeight(patError*& err) const ;


  virtual patString print() const ;

  // Returns an indicator that will be used to collect statistics
  // about the Markov process, in particular its stationarity. The
  // current implementation simply retiurns the weight. It should be
  // thought more thoroughly in the future.

  virtual patULong getIndicator(patError*& err) const ;

  patPath getSubpath(patError*& err) ;

  patULong getIndexOfNodeC(patError*& err) ;
  
  // Create a new state by replacing the subpath between the upstream
  // node and the downstream node by the specificed path Note: it
  // creates a new object. The caller is responsible to release the
  // memory.
  bioMhPathGenState* insertPath(patPath newSubpath,patError*& err) ;

  patULong getUserU() const ;
  patULong getUserV() const ;

  // A state is valid if the path connects the OD, and the
  // intermediate node is actually on the path

  pair<patBoolean,patString> isValidState() const ;

public:
  patPath thePath ;

  patNetwork* theNetwork ;
  patULong u ;
  patULong v ;

  patNode* nodeC ;
  patReal theScaleDetour ;
  patReal theScaleImportanceSampling ;
  patReal weight ;

};



#endif
