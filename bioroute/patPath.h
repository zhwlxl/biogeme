//----------------------------------------------------------------
// File: patPath.h
// Author: Michel Bierlaire
// Creation: Mon Nov 17 22:25:11 2008
//----------------------------------------------------------------

#ifndef patPath_h
#define patPath_h

#include <list>
#include <map>
#include "patType.h"
#include "patOd.h"
#include "patError.h"
#include "patIterator.h"

class patNode ;
class patArc ;
class patNetwork ;

class patPath {
  friend class patNetwork ;
  friend ostream& operator<<(ostream& str, const patPath& x) ;
  friend patBoolean operator<(const patPath& p1, const patPath& p2) ;

 public:
  patString name ;
  patOd od ;

  /**
     List of the user id of the nodes
   */
  vector<patULong> listOfNodes ;

 public:
  patPath() ;

  // Compute the index of the node aNode in the path, searching for it between indices deb and end. If the full path must be searched, deb=0, end=pathlength.

  patULong getNodeIndex(patNode* aNode, 
			patULong deb, 
			patULong end, patError*& err) ;

  patBoolean checkOd() ;

  void computeLinkAdditiveAttributes(patNetwork* theNetwork, patError*& err);
  
  void addAttribute(patString name, patReal value, patError*& err) ;

  patIterator<patArc*>* getLinkIterator(patNetwork* aNetwork) ;

  /**
     Extract the subpath from node index u to node index d. 
   */
  patPath extractSubPath(patULong uNode, patULong dNode,patError*& err) ;

  /**
     The two following functions are meant to merge two paths. If they
     don't have a common node, an error is triggered.  The list of
     attributes is emptied, as no default way of generating attributes
     for merged path is satisfactory.
   */
  void pushFront(patPath p, patError*& err) ;
  void pushBack(patPath p, patError*& err) ;
  void pushBack(patArc* a, patError*& err) ;

  /**
     This function erase all the nodes between firstNode and lastNode,
     and replace them by path p.

     Example: 
     current path : 1 2 3 4 5 6 7 8
     firstNode = 3
     latNode = 5
     path p:  3 10 20 30 40 50 5 
     new path: 1 2 3 10 20 30 40 50 5 6 7 8
     The OD of the path is not modified. The validity of the new path is not verified.
   */
  void overwrite(patPath p,
		 patULong indexFirstNode,
		 patULong indexLastNode,
		 patError*& err) ;

  patReal getGeneralizedCost(patNetwork* aNetwork, patError*& err) ;
  
  // Returns patTRUE if the node belongs to the path
  patBoolean contains(patULong nodeUserId) const ;

  // Deviation = exp(scale * (shortestPathCost - pathCost))

  patReal computeDeviationFromSpPath(patNetwork* aNetwork, 
				     patReal scale,
				     patError*& err) ;
  patReal getDeviation() const ;
 protected:
  map<patString,patReal> attributes ;

  patReal deviation ;
};
#endif 
