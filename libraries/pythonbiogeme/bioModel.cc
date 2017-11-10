//-*-c++-*------------------------------------------------------------
//
// File name : bioModel.cc
// Author :    Michel Bierlaire and Mamy Fetiarison
// Date :      Thu May  7 13:36:48 WEST 2009
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "bioModel.h"
#include "bioArithLiteral.h"
#include "patDisplay.h"
#include "bioExpressionRepository.h"
#include "bioLiteralRepository.h"

bioModel::bioModel(): theFormula(patBadId), 
		      statistics(NULL), 
		      formulas(NULL), 
		      constraints(NULL), 
		      simulation(NULL) ,
		      excludeExpr(patBadId),
		      weightExpr(patBadId),
		      theRepository(NULL) ,
		      varCovarForSensitivity(NULL),
		      performSensitivityAnalysis(patFALSE),
		      theBayesianExpr(NULL),
		      oldBetas(NULL) {
}

void bioModel::setRepository(bioExpressionRepository* rep) {
  theRepository = rep ;
}

void bioModel::setFormula(patULong f) {
  theFormula = f ;
}

patULong bioModel::getFormula() {
  return theFormula ;
}

void bioModel::setExcludeExpr(patULong e) {
  excludeExpr = e ;
}

patULong bioModel::getExcludeExpr() {
  return excludeExpr ;
}

void bioModel::setWeightExpr(patULong w) {
  weightExpr = w ;
}

patULong bioModel::getWeightExpr() {
  return weightExpr ;
}

void bioModel::setStatistics(map<patString, patULong>* stat) {
  statistics = stat ;
}

void bioModel::setFormulas(map<patString, patULong>* f) {
  formulas = f ;
}
 
void bioModel::setConstraints(map<patString, patULong>* constr) {
  constraints = constr ;
}

void bioModel::setSimulation(bioArithPrint* simul) {
  simulation = simul ;
}

void bioModel::setBayesian(bioArithBayes* b) {
  theBayesianExpr = b ;
}


map<patString, patULong>* bioModel::getStatistics() {
  return statistics ;
}

map<patString, patULong>* bioModel::getFormulas() {
  return formulas ;
}

map<patString, patULong>* bioModel::getConstraints() {
  return constraints ;
}

vector<pair<patString, patULong> >* bioModel::getUserExpressions() {
  return &userExpressions ;
}

bioArithPrint* bioModel::getSimulation() {
  return simulation ;
}



patBoolean bioModel::mustEstimate() const {
  patError* err(NULL) ;
  if (theFormula == patBadId) {
    return patFALSE ;
  }
  bioExpression* f = theRepository->getExpression(theFormula) ;
  if (f == NULL) {
    return patFALSE ;
  }
  if (f->getExpression(err) == "0") {
    return patFALSE ;
  }
  if (err != NULL) {
    return patFALSE ;
  }
  return patTRUE  ;
}

patBoolean bioModel::mustBayesEstimate() const {
  return (theBayesianExpr != NULL) ;
}


patBoolean bioModel::mustSimulate() const {
  return (simulation != NULL) ;
}

bioExpressionRepository* bioModel::getRepository() {
  return theRepository ;
}

void bioModel::addUserExpression(patString name, patULong exprId) {
  userExpressions.push_back(pair<patString, patULong>(name,exprId)) ;
}

void bioModel::setSensitivityAnalysis(vector<patString> params, 
				      patHybridMatrix* simul) {
  varCovarForSensitivity = simul ;
  namesForSensitivity = params ;
  performSensitivityAnalysis = patTRUE ;
}

patBoolean bioModel::mustPerformSensitivityAnalysis() const {
  return performSensitivityAnalysis ;
}

patHybridMatrix* bioModel::getVarCovarForSensitivity() {
  return varCovarForSensitivity ;
}

vector<patString> bioModel::getNamesForSensitivity() {
  return namesForSensitivity ;
}

bioArithBayes* bioModel::getBayesian() {
  return theBayesianExpr ;
}

patBoolean bioModel::involvesMonteCarlo() {
  if (theRepository == NULL) {
    return patFALSE ;
  }
  bioExpression* theExpr = theRepository->getExpression(theFormula) ;
  if (theExpr == NULL) {
    return patFALSE ;
  }
  return theExpr->containsMonteCarlo() ;
}


void bioModel::setOldBetas(map<patString,vector<patReal> >* r) {
  oldBetas = r ;
}

vector<patVariables>* bioModel::getOldBetas(patError*& err) {
  if (oldBetas == NULL) {
    return NULL ;
  }
  patULong size = 0 ;
  vector<patVariables>* result = new vector<patVariables> ;
  // Check if the lengths are consistent
  for (map<patString,vector<patReal> >::iterator i = oldBetas->begin() ;
       i != oldBetas->end() ;
       ++i) {
    patString betaName = i->first ;
    bioLiteralRepository::the()->getBetaValue(betaName, err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return NULL ;
    }
    
    if (size == 0) {
      size = i->second.size() ;
    }
    else {
      if (i->second.size() != size) {
	stringstream str ;
	str << "The number of old values for " << betaName << " should be " << size << " and not " <<  i->second.size() ;
	err = new patErrMiscError(str.str()) ;
	WARNING(err->describe()) ;
	return NULL ;
      }
    }
  }

  for (patULong k = 0 ; k < size ; ++k) {
    for (map<patString,vector<patReal> >::iterator i = oldBetas->begin() ;
	 i != oldBetas->end() ;
	 ++i) {
      patString betaName = i->first ;
      bioLiteralRepository::the()->setBetaValue(betaName, i->second[k], err) ;
      if (err != NULL) {
	WARNING(err->describe()) ;
	return NULL ;
      }
    }
    patVariables x = bioLiteralRepository::the()->getBetaValues(patFALSE) ;
    DEBUG_MESSAGE("Old solution " << k << ": " << x) ;
    result->push_back(x) ;
  }
  return result ;
}
