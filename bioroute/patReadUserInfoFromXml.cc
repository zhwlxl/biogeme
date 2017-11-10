//----------------------------------------------------------------
// File: patReadUserInfoFromXml.cc
// Author: Michel Bierlaire
// Creation: Sun Jun  7 10:26:11 2009
//----------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <xmlwrapp/xmlwrapp.h>
#include "patReadUserInfoFromXml.h"
#include "patDisplay.h"
#include "patErrMiscError.h"
#include "patRandomWalk.h"
#include "patMonteCarlo.h"
#include "bioPathMetropolisHastings.h"

patReadUserInfoFromXml::patReadUserInfoFromXml(patString fName) : 
  fileName(fName), 
  theNetwork(fName) {
  
}

patBoolean patReadUserInfoFromXml::readFile(patError*& err) {


  DEBUG_MESSAGE("Read " << fileName) ;
  xml::init xmlinit ;  
  
  xml::tree_parser* parser_ptr = new xml::tree_parser(fileName.c_str()); 
  if (!(*parser_ptr)) {
    stringstream str ;
    str << "Error while parsing " << fileName << " " << parser_ptr->get_error_message();
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe());
    return patFALSE ;
  }

  xml::document &doc = parser_ptr->get_document(); 

  xml::node theRoot = doc.get_root_node() ;

  xml::node::node_type theType = theRoot.get_type() ;

  xml::node::iterator rootIter(theRoot.begin()) ;
  xml::node::iterator rootEnd(theRoot.end()) ;

  map<patString,short> theNodeNames ;
  map<xml::node::node_type,short> theNodeTypes ;

  for (; rootIter != rootEnd ; ++rootIter) {
    patString nodeName(rootIter->get_name()) ;
    theNodeNames[nodeName]++ ;
    xml::node::node_type theType = rootIter->get_type() ;
    theNodeTypes[theType]++ ;
    if (nodeName == "network") {
      xml::attributes attr = rootIter->get_attributes() ;
      patULong id ;
      patString fileName ;
      patString networkName ;
      patReal arrowSpan ;
      for (xml::attributes::iterator  i = attr.begin() ; i != attr.end() ; ++i) { 
	patString attrName(i->get_name()) ;
	if (attrName == "file") {
	  fileName = patString(i->get_value()) ;
	}
	else if (attrName == "name") {
	  networkName = patString(i->get_value()) ; 
	}
	else if (attrName == "arrowSpan") {
	  arrowSpan = atof(i->get_value()) ;
	}
      }
      patReadNetworkFromXml theNetworkParser(fileName,
					     networkName,
					     cleanTheNetwork) ;
      theNetworkParser.readFile(err) ;
      if (err != NULL) {
	WARNING(err->describe()) ;
	return patFALSE ;
      }
      theNetwork = theNetworkParser.getNetwork() ;
      theNetwork.setArrowSpan(arrowSpan) ;
    }
    else if (nodeName == "doNotCleanTheNetwork") {
      xml::attributes attr = rootIter->get_attributes() ;
      patULong id ;
      for (xml::attributes::iterator i = attr.begin() ; i != attr.end() ; ++i) { 
      patString attrName(i->get_name()) ;
	if (attrName == "value") {
	  patString theValue(i->get_value()) ;
	  if (theValue == "1" || theValue == "true" || theValue == "TRUE") {
	    cleanTheNetwork = patFALSE ;
	  }
	  else {
	    cleanTheNetwork = patTRUE ;
	  }
	  break ;
	} 
      }

    }
    else if (nodeName == "od") {
      patULong orig ;
      patULong dest ;
      xml::attributes attr = rootIter->get_attributes() ;
      for (xml::attributes::iterator i = attr.begin() ; i != attr.end() ; ++i) { 
	patString attrName(i->get_name()) ;
	if (attrName == "orig") {
	  orig = atoi(i->get_value()) ;
	}
	if (attrName == "dest") {
	  dest = atoi(i->get_value()) ;
	}
      }
      theNetwork.addOd(orig,dest) ;

      for (xml::node::iterator  odSpecIter = rootIter->begin() ; 
	   odSpecIter != rootIter->end() ; 
	   ++odSpecIter) {
	patString tagName(odSpecIter->get_name()) ;
	if (tagName == "displayShortestPath") {
	  theNetwork.addShortestPathToDisplay(orig,dest) ;
	}
      }

    }

    else if (nodeName == "way") {
      patULong id ;
      patULong upNodeId ;
      patULong downNodeId ;
      patString theName ;
      xml::attributes attr = rootIter->get_attributes() ;
      for (xml::attributes::iterator i = attr.begin() ; i != attr.end() ; ++i) { 
	patString attrName(i->get_name()) ;
	if (attrName == "id") {
	  id = atoi(i->get_value()) ;
	  stringstream str ;
	  str << "Link " << id ;
	  theName = patString(str.str()) ;
	  break ;
	} 
      }

      for (xml::node::iterator linkIter = rootIter->begin() ; 
	   linkIter != rootIter->end() ; 
	   ++linkIter) {
	patString tagName(linkIter->get_name()) ;
	if (tagName == "bioattr") {
	  //	  DEBUG_MESSAGE("List of attributes");
	  pair<patString,patReal> oneAttribute ;
	  xml::attributes attr = linkIter->get_attributes() ;
	  for (xml::attributes::iterator i = attr.begin() ; i != attr.end() ; ++i) { 
	    if (patString(i->get_name()) == "name") {
	      oneAttribute.first = patString(i->get_value()) ;
	    } 
	    else if (patString(i->get_name()) == "value") {
	      oneAttribute.second = atof(i->get_value()) ;
	      theNetwork.addLinkAttributes(id,oneAttribute,err) ;
	      if (err != NULL) {
		WARNING(err->describe()) ;
		return patFALSE ;
	      }
	    }
	  }	  
	}
      }
    }
    else if (nodeName == "biosubpath") {
      xml::attributes attr = rootIter->get_attributes() ;
      patULong id ;
      for (xml::attributes::iterator i = attr.begin() ; i != attr.end() ; ++i) { 
	patString attrName(i->get_name()) ;
	if (attrName == "id") {
	  id = atoi(i->get_value()) ;
	  break ;
	} 
      }
      list<patULong> subpathIds ;
      
      for (xml::node::iterator spIter = rootIter->begin() ; 
	   spIter != rootIter->end() ; 
	   ++spIter) {
	patString tagName(spIter->get_name()) ;
	if (tagName == "ls") {
	  patULong spid = atoi(spIter->get_attributes().begin()->get_value()) ;
	  subpathIds.push_back(spid) ;
	}
      }
      theNetwork.addSubpath(id,subpathIds) ;
    }
    else if (nodeName == "biopath") {
      xml::attributes attr = rootIter->get_attributes() ;
      patULong id ;
      for (xml::attributes::iterator i = attr.begin() ; i != attr.end() ; ++i) { 
	patString attrName(i->get_name()) ;
	if (attrName == "id") {
	  id = atoi(i->get_value()) ;
	  break ;
	} 
      }
      list<patULong> subpathIds ;
      
      for (xml::node::iterator spIter = rootIter->begin() ; 
	   spIter != rootIter->end() ; 
	   ++spIter) {
	patString tagName(spIter->get_name()) ;
	if (tagName == "ls") {
	  patULong spid = atoi(spIter->get_attributes().begin()->get_value()) ;
	  subpathIds.push_back(spid) ;
	}
      }
      theNetwork.addPath(id,subpathIds) ;
    }
    else if (nodeName == "bioobs") {
      for (xml::node::iterator iter = rootIter->begin() ; 
	   iter != rootIter->end() ; 
	   ++iter) {
	patString tagName(iter->get_name()) ;
	if (tagName == "choice") {
	  
	}      
	else if (tagName == "biochar") {
	  
	}      
      }
    }
    else if (nodeName == "setOfPaths") {
      for (xml::node::iterator iter = rootIter->begin() ; 
	   iter != rootIter->end() ; 
	   ++iter) {
	patString tagName(iter->get_name()) ;
	if (tagName == "userDefined") {
	  patString name ;
	  xml::attributes attr = iter->get_attributes() ;
	  for (xml::attributes::iterator i = attr.begin() ; i != attr.end() ; ++i) { 
	    patString attrName(i->get_name()) ;
	    if (attrName == "name") {
	      name = patString(i->get_value()) ;
	    }
	  }   
	}
	else if (tagName == "randomWalk") {
	  patReal kumA, kumB ;
	  patULong nIter ;
	  patString name ;
	  xml::attributes attr = iter->get_attributes() ;
	  for (xml::attributes::iterator i = attr.begin() ; i != attr.end() ; ++i) { 
	    patString attrName(i->get_name()) ;
	    if (attrName == "kumA") {
	      kumA = atof(i->get_value()) ;
	    }
	    else if (attrName == "kumB") {
	      kumB = atof(i->get_value()) ;
	    }
	    else if (attrName == "nIter") {
	      nIter = atoi(i->get_value()) ;
	    }
	    else if (attrName == "name") {
	      name = patString(i->get_value()) ;
	    }
	  }
	  patRandomWalk* theAlgo = new patRandomWalk(kumA,kumB) ;
	  theAlgo->setMaximumNumberOfTrials(nIter) ;
	  theNetwork.addPathGenerationAlgo(name,theAlgo,err) ;
	  if (err != NULL) {
	    WARNING(err->describe());
	    return patFALSE ;
	  }
	}
	else if (tagName == "metropolisHastings") {
	  patReal kumA(1.0), kumB(1.0) ;
	  patULong simulationInterval(100) ;
	  patULong nIter ;
	  patULong warmup ;
	  patReal scaleDetour(0.0) ;
	  patReal scaleImportanceSampling(0.0) ;
	  patString name ;
	  patULong seed(0) ;
	  xml::attributes attr = iter->get_attributes() ;
	  for (xml::attributes::iterator i = attr.begin() ; i != attr.end() ; ++i) { 
	    patString attrName(i->get_name()) ;
	    if (attrName == "kumA") {
	      kumA = atof(i->get_value()) ;
	    }
	    else if (attrName == "kumB") {
	      kumB = atof(i->get_value()) ;
	    }
	    else if (attrName == "nIter") {
	      nIter = atoi(i->get_value()) ;
	    }
	    else if (attrName == "warmup") {
	      warmup = atoi(i->get_value()) ;
	    }
	    else if (attrName == "name") {
	      name = patString(i->get_value()) ;
	    }
	    else if (attrName == "simInterval") {
	      simulationInterval = atoi(i->get_value()) ;
	    } 
	    else if (attrName == "scaleDetour") {
	      scaleDetour = atof(i->get_value()) ;
	    }
	    else if (attrName == "scaleImportanceSampling") {
	      scaleImportanceSampling = atof(i->get_value()) ;
	    }
	    else if (attrName == "kumA") {
	      kumA = atof(i->get_value()) ;
	    }
	    else if (attrName == "kumB") {
	      kumB = atof(i->get_value()) ;
	    }
	    else if (attrName == "seed") {
	      seed = atoi(i->get_value()) ;
	    }
	  }


	  DEBUG_MESSAGE("Create the MH algo") ;
	  bioPathMetropolisHastings* theAlgo = 
	    new bioPathMetropolisHastings(name,
					  warmup,
					  simulationInterval,
					  kumA,
					  kumB,
					  scaleDetour,
					  scaleImportanceSampling, 
					  seed) ;
	  
	  theAlgo->setMaximumNumberOfTrials(nIter) ;
	  theNetwork.addPathGenerationAlgo(name,theAlgo,err) ;
	  if (err != NULL) {
	    WARNING(err->describe());
	    return patFALSE ;
	  }
	  
	}
	else if (tagName == "monteCarlo") {
	  patReal stdErr ;
	  patULong nIter ;
	  patString name ;
	  patULong seed(0) ;
	  xml::attributes attr = iter->get_attributes() ;
	  for (xml::attributes::iterator i = attr.begin() ; i != attr.end() ; ++i) { 
	    patString attrName(i->get_name()) ;
	    if (attrName == "stdErr") {
	      stdErr = atof(i->get_value()) ;
	    }
	    else if (attrName == "nIter") {
	      nIter = atoi(i->get_value()) ;
	    }
	    else if (attrName == "seed") {
	      seed = atoi(i->get_value()) ;
	    }
	    else if (attrName == "name") {
	      name = patString(i->get_value()) ;
	    }
	  }
	  patMonteCarlo* theAlgo = new patMonteCarlo(seed) ;
	  theAlgo->setStandardError(stdErr) ;
	  theAlgo->setMaximumNumberOfTrials(nIter) ;
	  theNetwork.addPathGenerationAlgo(name,theAlgo,err) ;
	  if (err != NULL) {
	    WARNING(err->describe());
	    return patFALSE ;
	  }
	}
      }
    }
    else if (nodeName == "computePathAttributes") {
      for (xml::node::iterator compPathIter = rootIter->begin() ; 
	   compPathIter != rootIter->end() ; 
	   ++compPathIter) {
	patString tagName(compPathIter->get_name()) ;
	if (tagName == "leftTurns") {
	  xml::attributes attr = compPathIter->get_attributes() ;
	  for (xml::attributes::iterator i = attr.begin() ; i != attr.end() ; ++i) { 
	    patString attrName(i->get_name()) ;
	    if (attrName == "minAngle") {
	      patAngle theAngle ;
	      theAngle.setAngleInDegree(atof(i->get_value())) ; 
	      theNetwork.mustComputeLeftTurns(patTRUE) ;
	      theNetwork.setThresholdForLeftTurns(theAngle) ;
	      break ;
	    } 
	  }
	}
      }
    }
  }

  if (cleanTheNetwork) {
    DEBUG_MESSAGE("Before cleaning: " << theNetwork.nbrOfArcs() << " arcs and " <<theNetwork.nbrOfNodes() << " nodes") ;
  }

  theNetwork.finalizeNetwork(cleanTheNetwork,err) ;

  if (err != NULL) {
    WARNING(err->describe());
    return patFALSE ;
  }

  if (cleanTheNetwork) {
    DEBUG_MESSAGE("After cleaning: " << theNetwork.nbrOfArcs() << " arcs and " <<theNetwork.nbrOfNodes() << " nodes") ;
  }
  return patTRUE ;
  

}
  

patNetwork patReadUserInfoFromXml::getNetwork() {
  return theNetwork ;

}
