//-*-c++-*------------------------------------------------------------
//
// File name : bioMarkovState.cc
// Author :    Michel Bierlaire
// Date :      Mon Aug  9 15:23:11 2010
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <sstream>
#include "bioMarkovState.h"

bioMarkovState::bioMarkovState()  {

}


patString bioMarkovState::print() const {
  stringstream str ;
  patError* err(NULL) ;
  str << getIndicator(err) ;
  return patString(str.str()) ;
}
