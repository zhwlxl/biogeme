//-*-c++-*------------------------------------------------------------
//
// File name : bioMhShuffleTransition.h
// Author :    Michel Bierlaire
// Date :      Wed Aug 11 14:53:51 2010
//
//--------------------------------------------------------------------

#ifndef bioMhShuffleTransition_h
#define bioMhShuffleTransition_h

#include <map>
#include "patRandomInteger.h"
#include "bioMarkovProcess.h"

class bioMhDetourTransition ;
class bioMhPathGenState ;
class patUniform ;

/**


                             c1 --->  c2

Orig --> x --->  u1 ---> u2 ---> x ---> x ---> v1 ---> v2 ---> x ---> Dest


A state is a combination of 
- a path p
- two links on the paths: (u1,u2) and (v1,v2) such that u2 is upstream of v1, or u2 = v1
- a link (c1,c2) anywhere on the network


The Markov process  applies two types of transitions:
- Select randomly the u and v links on the path  [this transition]
- Select link c such that it is not "too far" from u and v, and
   - Connect u2 and c1 with a shortest path
   - Connect c2 and v1 with a shortest path


*/


class patNetwork ;

class bioMhShuffleTransition : public bioMarkovProcess {
 public:
  bioMhShuffleTransition(patNetwork* n, 
			 patReal scale,
			 patUniform* rng) ;
  virtual bioMarkovTransition getNextState(bioMarkovState* currentState, 
					   patError*& err) ;

 private:  

  bioMhDetourTransition* theDetourProcess ;
  bioMhPathGenState* currentState ;
  patNetwork* theNetwork ;
  patReal theScale ;
  patUniform* theRng ;
  patRandomInteger drawRandomInteger ;
};
#endif
