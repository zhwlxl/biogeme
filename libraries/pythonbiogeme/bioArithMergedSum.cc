//-*-c++-*------------------------------------------------------------
//
// File name : bioArithMergedSum.cc
// Author :    Michel Bierlaire and Mamy Fetiarison
// Date :      Thu Aug  6 14:22:23 2009
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "patDisplay.h"
#include "bioArithLiteral.h"
#include "bioArithMergedSum.h"
#include "patErrMiscError.h"
#include "patErrNullPointer.h"
#include "bioIteratorInfo.h"
#include "bioLiteralRepository.h"
#include "bioParameters.h"

// results contaibns the name of the variables where the results must
// be stored in the generated code
bioArithMergedSum::bioArithMergedSum(bioExpression* par,
				     bioIteratorInfo* aIterator,
				     vector<patString> results,
				     vector<bioExpression*> listOfExpressions) :
  bioExpression(par,NULL,NULL),
  theIteratorInfo(aIterator),
  variablesNames(results),
  allExpressions(listOfExpressions) {
  
}

bioArithMergedSum::~bioArithMergedSum() {
  
}

bioExpression::bioOperatorType bioArithMergedSum::getOperatorType() const {
  return bioExpression::MERGED_OP ;
}

patString bioArithMergedSum::getOperatorName() const {
  return patString("MergedSum") ;
}

patReal bioArithMergedSum::getValue(patError*& err) const {
  err = new patErrMiscError("Cannot compute the value of an iterator") ;
  WARNING(err->describe()) ;
  return patReal() ;
}

bioExpression* bioArithMergedSum::getDerivative(patULong aLiteralId, 
						patError*& err) const {
  vector<bioExpression*> theDerivatives ;
  for (vector<bioExpression*>::const_iterator i = allExpressions.begin() ;
       i != allExpressions.end() ;
       ++i) {
    bioExpression* aDeriv = (*i)->getDerivative(aLiteralId,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return NULL ;
    }
    theDerivatives.push_back(aDeriv) ;
  }
  vector<patString> theDerivResult ;
  patString literal = bioLiteralRepository::the()->getName(aLiteralId,err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return NULL ;
  }
  for (vector<patString>::const_iterator i = variablesNames.begin() ;
       i != variablesNames.end() ;
       ++i) {
    stringstream str ;
    str << *i << "_" << literal ;
    theDerivResult.push_back(patString(str.str())) ;
  }
  bioExpression* result = new bioArithMergedSum(NULL,
						theIteratorInfo,
						theDerivResult,
						theDerivatives) ;
  return result ;

}

bioArithMergedSum* bioArithMergedSum::getDeepCopy(patError*& err) {
  vector<bioExpression*> theCopies ;
  for (vector<bioExpression*>::iterator i = allExpressions.begin() ;
       i != allExpressions.end() ;
       ++i) {
    bioExpression* aCopy = (*i)->getDeepCopy(err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return NULL ;
    }
    theCopies.push_back(aCopy) ;
  }
  bioArithMergedSum* result = new bioArithMergedSum(NULL,
						theIteratorInfo,
						variablesNames,
						theCopies) ;
  return result ;
  
}

patString bioArithMergedSum::getExpression(patError*& err) const {
  if (variablesNames.size() != allExpressions.size()) {
    stringstream str ;
    str << "Inconsistent sizes: " << variablesNames.size() << "!=" << allExpressions.size() ;
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe()) ;
    return patString() ;
  }
  stringstream str ;
  str << theIteratorInfo->getInfo() << "[" ;
  for (patULong i = 0 ; i < variablesNames.size() ; ++i) {
    if (i != 0) {
      str << ", " ;
    }
    str << variablesNames[i] << "=" << allExpressions[i]->getExpression(err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return patString() ;
    }
  }
  str << "]" ;
  return patString(str.str()) ;
}

patBoolean bioArithMergedSum::dependsOf(patULong aLiteralId) const {
  for (vector<bioExpression*>::const_iterator i = allExpressions.begin() ;
       i != allExpressions.end() ;
       ++i) {
    if ((*i)->dependsOf(aLiteralId)) {
      return patTRUE ;
    }
  }
  return patFALSE ;
}
void bioArithMergedSum::simplifyZeros(patError*& err) {
  for (vector<bioExpression*>::const_iterator i = allExpressions.begin() ;
       i != allExpressions.end() ;
       ++i) {
    (*i)->simplifyZeros(err);
    if (err != NULL) {
      WARNING(err->describe()) ;
      return ;
    }
  }
}


bioIteratorInfo* bioArithMergedSum::theIterator() {
  return theIteratorInfo ;
}

patULong bioArithMergedSum::getNumberOfOperations() const {
  patULong result(0) ;
  for (vector<bioExpression*>::const_iterator i = repeatedExpressions.begin() ;
       i != repeatedExpressions.end() ;
       ++i) {
    result += (*i)->getNumberOfOperations() ;
  }

  for (vector<bioExpression*>::const_iterator i = allExpressions.begin() ;
       i != allExpressions.end() ;
       ++i) {
    result += (*i)->getNumberOfOperations() ;
  }
  //  result *= theIteratorInfo->mapIds.size() ;
  return result ;
}

patString bioArithMergedSum::getIncludeStatements(patString prefix) const {
  stringstream str ;
  for (vector<bioExpression*>::const_iterator i = allExpressions.begin() ;
       i != allExpressions.end() ;
       ++i) {
    str <<  (*i)->getIncludeStatements(prefix) << endl ;
  }
  for (patULong i=0 ; i < allExpressions.size() ; ++i) {
    str << "patReal "<< prefix <<"__file" << i << "(bioIterates* theData,vector<patReal>* commonExpressions) ;" << endl ;
  }
  return patString(str.str()) ;
}


patBoolean bioArithMergedSum::containsAnIterator() const {
  return patTRUE ;
}

patBoolean bioArithMergedSum::containsASequence() const {
  for (vector<bioExpression*>::const_iterator i = allExpressions.begin() ;
       i != allExpressions.end() ;
       ++i) {
    if  ((*i)->containsASequence()) {
      return patTRUE ;
    }
  }
  return patFALSE ;

}

patBoolean bioArithMergedSum::containsAnIntegral() const {
  for (vector<bioExpression*>::const_iterator i = allExpressions.begin() ;
       i != allExpressions.end() ;
       ++i) {
    if  ((*i)->containsAnIntegral()) {
      return patTRUE ;
    }
  }
  return patFALSE ;

}

bioExpression* bioArithMergedSum::getExpressionFromId(patULong theId) {
  DEBUG_MESSAGE("Look for expression " << theId) ;
  if (theId == getId()) {
    return this ;
  }
  bioExpression* result(NULL) ;
  for (vector<bioExpression*>::iterator i = allExpressions.begin() ;
       i != allExpressions.end() ;
       ++i) {
    result = (*i)->getExpressionFromId(theId) ;
    if (result != NULL) {
      return result ;
    }
  }
  return NULL ;
  
}

patULong bioArithMergedSum::countExpressions(map<patString,patULong>* expr, 
					     map<patString,list<patULong> >* theIds,
					     map<patString,list<bioExpression*> >* thePointers,
					     patError*& err)   {
    stringstream str ;
    str << "Expression to simplify cannot contain an iterator: " << *this;
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe()) ;
    return 0;
}


void bioArithMergedSum::replaceExpression(bioExpression* theExpression, 
					  bioExpression* anotherExpression, 
					  patError*& err) {
  
  if (*this == *theExpression) {
    stringstream str ;
    str << "Cannot replace the root of an expression" ;
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe());
    return ;
  }

  for (vector<bioExpression*>::size_type i = 0 ; 
       i < allExpressions.size() ;
       ++i) {
    //    DEBUG_MESSAGE("BEFORE: Expr["<<i<<"]="<<*(allExpressions[i])) ;
    if (*(allExpressions[i]) == *theExpression) {
      //      DELETE_PTR(*i) ;
      allExpressions[i] = anotherExpression->getDeepCopy(err) ;
      if (err != NULL) {
	WARNING(err->describe()) ;
	return ;
      }
      //allExpressions[i]->setParent(this) ;
      return ;
    }
    else {
      allExpressions[i]->replaceExpression(theExpression,
					   anotherExpression,
					   err) ;
      if (err != NULL) {
	WARNING(err->describe()) ;
	return ;
      }
    }
    //    DEBUG_MESSAGE("AFTER: Expr["<<i<<"]="<<*(allExpressions[i])) ;
  }
}

patString bioArithMergedSum::getExpressionString() const {
  stringstream str ;
  str << "$Z" ;
  str << theIteratorInfo->iteratorName ;
  str << '[' ;
  for (vector<bioExpression*>::const_iterator i = allExpressions.begin() ;
       i != allExpressions.end() ;
       ++i) {
    str << '{' << (*i)->getExpressionString() << '}' ;
  }
  str << ']' ;
  return patString(str.str()) ;
}

void bioArithMergedSum::getExpressionsToSimplify(vector<bioExpression*>* listOfExpr, 
						 patError*& err) {

  // The merged iterator will be added to the list if at least one of
  // its subpexression does not contain an iterator. At the time of
  // simplification, only expression without iterators will be
  // simplified

  patBoolean lowest(patFALSE) ;
  for (vector<bioExpression*>::const_iterator i = allExpressions.begin() ;
       i != allExpressions.end() ;
       ++i) {
    if ((*i)->containsAnIterator()) {
      (*i)->getExpressionsToSimplify(listOfExpr,err) ;
      if (err != NULL) {
	WARNING(err->describe()) ;
	return ;
      }
    }
    else {
      if (!lowest) {
	lowest = patTRUE ;
	listOfExpr->push_back(this) ;
      }
    }
  }
  
}

void bioArithMergedSum::tryToSimplify(patError*& err) {

  short simplifyExpressions = bioParameters::the()->getValueInt("simplifyExpressions",err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return ;
  }
  if (simplifyExpressions == 0) {
    return  ;
  }


  patULong originalNbrOper = getNumberOfOperations() ;
    patULong counter = 0 ;
  patBoolean finished = patFALSE ;
  while (!finished && counter <= 100) {
    ++counter ;

    map<patString,patULong> theSubExpressions ;
    map<patString,list<patULong> > theSubIds ;
    map<patString,list<bioExpression*> > thePointers ;
    for (vector<bioExpression*>::iterator expr = allExpressions.begin() ;
	 expr != allExpressions.end() ;
	 ++expr) {
      bioExpression* theExpression = *expr ;
      if (!theExpression->containsAnIterator()) {
	//	DEBUG_MESSAGE("No iterator. OK to simplify globally.") ;
	theExpression->countExpressions(&theSubExpressions,&theSubIds,&thePointers,err) ;
	if (err != NULL) {
	  WARNING(err->describe()) ;
	  return ;
	}
      }
      else {
	//	DEBUG_MESSAGE("Iterator. Simplify locally.") ;
	if (!theExpression->canBeSimplified()) {
	  vector<bioExpression*> expressionsToSimplify ;
	  theExpression->getExpressionsToSimplify(&expressionsToSimplify,err) ;
	  if (err != NULL) {
	    WARNING(err->describe()) ;
	    return ;
	  }
	  //	  DEBUG_MESSAGE("Number of expressions found: " << expressionsToSimplify.size()) ;
	  for (vector<bioExpression*>::iterator i = expressionsToSimplify.begin() ;
	       i != expressionsToSimplify.end() ;
	       ++i) {
	    (*i)->simplifyLowerIteratorOrSequence(err) ;
	    if (err != NULL) {
	      WARNING(err->describe()) ;
	      return ;
	    }
	  }
	}
	else {
	  // Simplify expression
	  bioExpression* newExpression = simplify(theExpression,err) ;
	  if (err != NULL) {
	    WARNING(err->describe()) ;
	    return ;
	  }
	  *expr = newExpression ;
	}
	
	if (err != NULL) {
	  WARNING(err->describe()) ;
	  return ;
	}
      }
    }
//   DEBUG_MESSAGE("SUBIDS") ;
//   for (map<patString,list<patULong> >::iterator k = theSubIds.begin() ;
//        k != theSubIds.end() ;
//        ++k) {
//     DEBUG_MESSAGE(k->first << ": ") ;
//     for (list<patULong>::iterator kk = k->second.begin() ;
// 	 kk != k->second.end() ;
// 	 ++kk) {
//       cout << *kk << " " ;
//     }
//     cout << endl ;
//   }
    
    patULong maxGain = 0 ;
    bioExpression* aRepeatedExpression(NULL) ;
    
    for (map<patString,patULong>::iterator i = theSubExpressions.begin() ;
	 i != theSubExpressions.end() ;
	 ++i) {
      
      if (i->second > 1) {
	//	DEBUG_MESSAGE(i->first << ": " << i->second) ;
	bioExpression* theExpr = *(thePointers[i->first].begin()) ;
	if (theExpr == NULL) {
	  err = new patErrNullPointer("bioExpression") ;
	  WARNING(err->describe()) ;
	  return ;
	}
	//	DEBUG_MESSAGE("Expression: " << *theExpr) ;
	patULong gain = theExpr->getNumberOfOperations() * (i->second-1) ;
	if (gain > maxGain) {
	  maxGain = gain ;
	  aRepeatedExpression = theExpr ;
	}
      }
    }
    if (maxGain > 0) {
      //      DEBUG_MESSAGE("Maxgain: " << maxGain << " Simplify: " << *aRepeatedExpression) ;
      repeatedExpressions.push_back(aRepeatedExpression) ;
      patULong theLiteralId = bioLiteralRepository::the()->addCompositeLiteral(err) ;
      if (err != NULL) {
	WARNING(err->describe()) ;
	return ;
      }
      bioArithLiteral* theSimplifiedExpression =  new bioArithLiteral(NULL,theLiteralId) ;
      shortExpressions.push_back(theSimplifiedExpression) ;
      
      //      DEBUG_MESSAGE("SIMPLIFIED EXPRESSION:" << *theSimplifiedExpression) ;
      replaceExpression(aRepeatedExpression,theSimplifiedExpression,err) ;  
      if (err != NULL) {
	WARNING(err->describe()) ;
	return ;
      }
    }
    else {
      finished = patTRUE ;
    }
    patULong currentNbrOper = getNumberOfOperations() ;

  }
  
}

void bioArithMergedSum::generateGslFunctions(ostream& str_h,patString prefix, patError*& err) {
  for (vector<bioExpression*>::const_iterator i = allExpressions.begin() ;
       i != allExpressions.end() ;
       ++i) {
    (*i)->generateGslFunctions(str_h,prefix,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return ;
    }
  }
}

