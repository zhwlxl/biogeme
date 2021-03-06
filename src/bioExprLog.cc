//-*-c++-*------------------------------------------------------------
//
// File name : bioExprLog.cc
// @date   Tue Apr 17 12:16:32 2018
// @author Michel Bierlaire
// @version Revision 1.0
//
//--------------------------------------------------------------------

#include "bioExprLog.h"
#include "bioDebug.h"
#include "bioExceptions.h"
#include <cmath>
#include "bioSmartPointer.h"
#include <sstream>

bioExprLog::bioExprLog(bioSmartPointer<bioExpression>  c) :
  child(c) {
  listOfChildren.push_back(c) ;
}

bioExprLog::~bioExprLog() {
}

bioSmartPointer<bioDerivatives>
bioExprLog::getValueAndDerivatives(std::vector<bioUInt> literalIds,
				   bioBoolean gradient,
				   bioBoolean hessian) {
  

  theDerivatives = bioSmartPointer<bioDerivatives>(new bioDerivatives(literalIds.size())) ;

  bioUInt n = literalIds.size() ;
  bioSmartPointer<bioDerivatives> childResult = child->getValueAndDerivatives(literalIds,gradient,hessian) ;
  if (childResult->f <= 0) {
    std::stringstream str ;
    str << "Current values of the literals: " << std::endl ;
    std::map<bioString,bioReal> m = getAllLiteralValues() ;
    for (std::map<bioString,bioReal>::iterator i = m.begin() ;
	 i != m.end() ;
	 ++i) {
      str << i->first << " = " << i->second << std::endl ;
    }
    if (rowIndex != NULL) {
      str << "row number: " << *rowIndex << ", ";
    }
    
    str << "Cannot take the log of a non positive number: log("
	<< childResult->f << ")" << std::endl ;
    throw bioExceptions(__FILE__,__LINE__,str.str()) ;
  }
  else {    
    theDerivatives->f = log(childResult->f) ;
  }
  if (gradient) {
    for (bioUInt i = 0 ; i < n ; ++i) {
      theDerivatives->g[i] = childResult->g[i] / childResult->f ;
      if (hessian) {
	for (bioUInt j = 0 ; j < n ; ++j) {
	  bioReal fsquare = childResult->f * childResult->f ;
	  theDerivatives->h[i][j] = childResult->h[i][j] / childResult->f -  childResult->g[i] *  childResult->g[j] / fsquare ;
	}
      }
    }
  }
  return theDerivatives ;
}

bioString bioExprLog::print(bioBoolean hp) const {
  std::stringstream str ; 
  str << "log(" << child->print(hp) << ")";
  return str.str() ;
}

