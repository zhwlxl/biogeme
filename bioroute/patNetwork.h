//----------------------------------------------------------------
// File: patNetwork.h
// Author: Michel Bierlaire
// Creation: Thu Oct 30 09:46:33 2008
//----------------------------------------------------------------

#ifndef patNetwork_h
#define patNetwork_h

#include <map>
#include <set>
#include "patNode.h"
#include "patArc.h"
#include "patPath.h"
#include "patError.h"
#include "patOd.h"
#include "patLinkSubpath.h"
#include "patListSubpath.h"
#include "patAngle.h"

class patPathGenerationAlgo ;
class patRandomNumberGenerator ;
class patNetwork {

 public:
  /**
   * Constructor
   * @param theName name of the network
   */

  patNetwork(patString theName) ;

  /**
     copy ctor
   */
  patNetwork(const patNetwork& aNetwork) ;

  /**
     assignment
   */
  patNetwork& operator=(const patNetwork& aNetwork) ;
  /**
   */

  /**
   * @param minLat latitude of the south-most point in the network
   * @param maxLat latitude of the north-most point in the network
   * @param minLon longitude of the west-most point in the network
   * @param maxLon longitude of the east-most point in the network
   */
  void setMapBounds(patReal minLat, 
		    patReal maxLat, 
		    patReal minLon, 
		    patReal maxLon) ;
  

  /**
     @param theNode node to be added 
     @return TRUE if the node has been
     added, FALSE otherwise. In particular, if the node already exists,
     it is not added.
   */
  patBoolean addNode(const patNode& theNode) ;

  /**
     @param theId  Unique arc identifier
     @param aNode Upstream node of the arc. If it does not exist in
     the network, it will be added.
     @param bNode Downstream node of the arc. If it does not exist in
     the network, it will be added.
     @param theName name of the arc
     @return TRUE is the arc has been added, FALSE otherwise.  
  */
  patBoolean addArc(patULong theId, 
		    patNode* aNode,
		    patNode* bNode, 
		    patString theName,
		    map<patString,patReal> attributes,
		    patError*& err) ;

  /**
     @param theId  Unique arc identifier
     @param aNodeId Id of the upstream node of the arc. 
     @param bNodeId ID of the downstream node of the arc.
     @param theName name of the arc
     @return TRUE is the arc has been added, FALSE otherwise.  
  */
  patBoolean addArcWithIds(patULong theId, 
			   patULong aNodeId,
		   patULong bNodeId, 
			   patString theName,
			   map<patString,patReal> attributes,
			   patError*& err) ;
  
  /**
     @param id  User ID
     @return Pointer to the node if it exists. NULL otherwise.
   */

  void addLinkAttributes(patULong id, 
			 pair<patString,patReal> attributes, 
			 patError*& err) ;

  /**
   */
  patNode* getNodeFromUserId(patULong id) ;

  /**
   */
  patNode* getNodeFromInternalId(patULong internalId) ;

  /**
   */
  patPath* getPathFromUserId(patULong id) ;


  /**
   */
  void addShortestPathToDisplay(patULong orig,patULong dest) ;

  /**
   */
  patString getName() const ;

  /**
     @param id  User ID
     @return Pointer to the arc if it exists. NULL otherwise.
   */
  patArc* getArcFromUserId(patULong id) ;

  /**
     @param id  Internal ID
     @return Pointer to the arc if it exists. NULL otherwise.
   */
  patArc* getArcFromInternalId(patULong id) ;


  /**
     @param aNodeId User ID of the upstream node
     @param bNodeId User ID of the downstream node
     @return Pointer to the arc if it exists. NULL otherwise.
   */
  patArc* getArcFromNodesUserId(patULong aNodeId, patULong bNodeId)  ;

  /**
     @param aNodeId User ID of a node
     @param bNodeId User ID of a node
     @return Number of arcs adjacent to both nodes. Typically 0 (no arc linking the nodes), 1 (one way), 2 (two ways). 
   */
  short isArcOneWay(patULong aNodeId, patULong bNodeId) const ;

  /**
     @param first User ID of an arc
     @param second User ID of an arc
     @return patTRUE is the two arcs are consecutive
   */
  patBoolean consecutiveArcs(patULong first, patULong second, patError*& err) ;

  /**
   */
  void setArcLength(patULong arcId, patReal l, patError*& err) ;

  /**
   */
  void computeArcLength(patULong arcId, patError*& err) ;



  /**
   */
  void setThresholdForLeftTurns(patAngle threshold) ;


  /**
   */
  patULong nbrOfArcs() const ;

  /**
   */
  patULong nbrOfNodes() const ;


  /**
   */
  void writeKML(patString fileName, patError*& err) ;


  /**
   */
  void addOd(patULong o, patULong d) ;

  /**
     @return patBadId if the node userId does not exists
   */
  patULong getInternalNodeIdFromUserId(patULong userId) const ;

  /**
     @return patBadId if the node userId does not exists
   */
  patULong getUserNodeIdFromInternalId(patULong internalId) const ;

  /**
     @return patBadId if the arc userId does not exists
   */
  patULong getInternalArcIdFromUserId(patULong userId) const ;

  /**
     This function should be cvalled after the network description has been loaded.
   */
  void finalizeNetwork(patBoolean cleanTheNetwork,patError*& err) ;

  void computeLinkAdditivePathAttributes(patError*& err) ;

  patReal getShortestPathCost(patOd od, patError*& err) ;

  patReal computePathCost(patPath p, patError*& err) ;

  patReal computeAngleNodes(patULong n1, patULong n2, patULong n3) ;

  void generatePaths(patError*& err) ;


  void restoreOriginalCosts(patError*& err) ;

  void generateMonteCarloCosts(patRandomNumberGenerator* aRndGen, 
			       patReal stdErr,
			       patBoolean enforcePositivity,
			       patError*& err) ;


  /**
     Returns the shortest path distance from (and to) the node nodeId
     to (from) any node in the network.
   */
  vector<pair<patReal,patReal> > getDistanceFromAndToNode(patNode* node, patError*& err) ;

  /**
     orig and dest are user IDs
   */
  patPath getShortestPath(patOd od, patBoolean* success, patError*& err) ;

  /**
     Add a subpath read from the file. 
   */
  void addSubpath(patULong id, list<patULong> subpathIds) ;

  /**
     Add a path read from the file. 
   */
  void addPath(patULong id, list<patULong> subpathIds) ;

  /**
   */
  patString displaySubpaths(patError*& err) ;

  /**
   */
  patIterator<patPath*>* getPathIterator() ;

  /**
   */
  patIterator<patNode*>* getNodeIterator() ;

  /**
   */
  patIterator<patArc*>* getArcIterator() ;


  /**
   */
  void mustComputeLeftTurns(patBoolean b) ;

  /**
   */
  void addPathGenerationAlgo(patString name, 
			     patPathGenerationAlgo* aAlgo, 
			     patError*& err) ;


  /**
   */
  patBoolean isPathValid(patPath p) ;


  /**
   */
  void createGnuplotFile(patString gnuplotFileName, 
			 patString networkFileName, 
			 patString pathFileName,
			 patError*& err) ;

  /**
   */

  void createPathStatistics(patString statFileName,patError*& err) ;

  void setArrowSpan(patReal as) ;

  void addPathFromMarkovProcess(patPath aPath) ;

 protected:

  /**
   */
  void computeLeftTurns(patError*& err) ;
  

  /**
   */
  int computeLeftTurns(patPath* aPath, patError*& err) ;

  /**
   */
  void copyObject(const patNetwork& aNetwork) ;
  /**
   */
  void buildAdjacencyLists(patError*& err) ;

  /**
   */
  void buildSubpaths(patError*& err) ;

  /**
   */
  void buildUserDefinedPaths(patError*& err) ;

  /**
   */
  patSubpath* createSubpath(patULong id, patError*& err) ;
  /**
   */
  void computeMinimalLabel(patError*& err) ;

  /**
   */
  void removeUselessNodes(patError*& err) ;
  /**
     @return patTRUE if the node has been removed. The iteratord may
     not be valid anymore.
   */
  patBoolean removeUselessNode(patULong id, patError*& err) ;

  /**
     Internal IDs are consecutive, and correspond to indices in a vector 
   */
  void computeInternalIds() ;
  /**
   */
  void registerOds(patError*& err) ;




  patString getKMLCode(patPath p, patString description, patString color) ;

  vector<patULong> transformListOfInternalIntoExternalIds(list<patULong> internalList, 
							patError*& err) ;

  

 protected:
  patString name ;
  map<patULong, patArc> theArcs ;
  map<patULong, patNode> theNodes ;
  vector<patArc*> internalArcs ;
  vector<patNode*> internalNodes ;
  set<patOd> theOds ;
  map<pair<patULong, patULong>, patULong > listOfArcsPerPairsOfNodes ;
  patReal minLatitude ;
  patReal maxLatitude ;
  patReal minLongitude ;
  patReal maxLongitude ;
  patReal minimumLabelForShortestPath ;
  vector< list < pair<patReal,patULong> > >* adjacencyLists ;
  map<patULong,patSubpath*> allSubpaths ;
  list<patLinkSubpath> theLinksSubpaths ;
  list<patListSubpath> theListSubpaths ;
  map<patULong,list<patULong> > tempListOfSubpaths ;
  map<patULong,list<patULong> > tempListOfPaths ;
  set<patOd> shortestPathsToBeDisplayed ;
  map<patULong,patPath> userDefinedPaths ;
  patAngle thresholdForLeftTurns ;
  patBoolean leftTurns ;
  map<patString,patPathGenerationAlgo*> listOfAlgos ;
  map<patString,list<patPath> > generatedPaths ;
  patReal arrowSpan ;
};
#endif
