//-*-c++-*------------------------------------------------------------
//
// File name : bioPrematureStop.cc
// Author :    Michel Bierlaire 
// Date :      Fri Aug  4 15:00:42 2017
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "patDisplay.h"
#include "bioPrematureStop.h"
#include "bioParameters.h"
#include "patConst.h"

bioPrematureStop::bioPrematureStop(vector<patVariables>* aList, patError*& err) : theList(aList) {
  threshold = bioParameters::the()->getValueReal("prematureConvergenceThreshold",err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return ;
  }
}

patBoolean bioPrematureStop::interruptIterations() {
  if (theList == NULL) {
    return patFALSE ;
  }
  if (threshold == 0.0) {
    return patFALSE ;
  }
  for (vector<patVariables>::iterator i = theList->begin() ;
       i != theList->end() ;
       ++i) {
    patVariables diff = (*i)-x ;
    DEBUG_MESSAGE("Compare " << *i << " and " << x) ;
    DEBUG_MESSAGE("Distance: " << norm2(diff)) ;
    if (norm2(diff) <= threshold) {
      neighbor = *i ;
      DEBUG_MESSAGE("Too close. Stop") ;
      return patTRUE ;
    }
      DEBUG_MESSAGE("Not close enough. Continue") ;
  }
  return patFALSE ;
}

patString bioPrematureStop::reasonForInterruption() {
  stringstream str ;
  str << "Algorithm reaching the vicinity of " << neighbor ;
  return patString(str.str()) ;
}
