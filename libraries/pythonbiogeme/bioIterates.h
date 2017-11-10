//-*-c++-*------------------------------------------------------------
//
// File name : bioIterates.h
// Author :    Michel Bierlaire and Mamy Fetiarison
// Date :      Sat Aug 15 15:48:18 2009
//
//--------------------------------------------------------------------

#ifndef bioIterates_h
#define bioIterates_h

#include <vector>
#include "patType.h"
#include "trVector.h" 
#include "patVariables.h" 
#include "bioIteratorSpan.h" 
#include "bioSample.h"

/*!  This data structure contains the possible iterates of the
  iterator, as well as the parameters
*/

class bioIterates {
public:
  bioIterates() : __x(NULL), aSpan(), __draw(NULL), __beta(NULL), sample(NULL) {} ;
  // Row iterator
  patVariables* __x ;
  // Meta iterator 
  bioIteratorSpan aSpan ;
  // Draw iterator
  vector< vector<patReal> >* __draw ;
  // Sample
  bioSample* sample ;
  // Parameters
  trVector* __beta ;
  // Thread span
  bioIteratorSpan __theThreadSpan ;
};
#endif
