//-*-c++-*------------------------------------------------------------
//
// File name : patShortestPathTreeDest.h
// Author :    \URL[Michel Bierlaire]{http://roso.epfl.ch/mbi}
// Date :      Tue May 16 15:07:07 2006
//
//--------------------------------------------------------------------

#ifndef patShortestPathTreeDest_h
#define patShortestPathTreeDest_h


#include <vector>
#include <list>
#include "patType.h"
#include "patError.h"


class patShortestPathTreeDest {

  friend class patShortestPathAlgoDest ;
  friend class patPathProcessor ;

public:
  patShortestPathTreeDest(patULong theDest, patULong nNodes) ;
  
  list<patULong> getShortestPathFrom(patULong node,patError*& err) ;
protected:
  patULong theDest ;
  vector<patReal> labels ;
  vector<patULong> successor ;
};

#endif
