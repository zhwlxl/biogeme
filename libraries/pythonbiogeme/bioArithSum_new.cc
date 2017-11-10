//-*-c++-*------------------------------------------------------------
//
// File name : bioArithSum.cc
// Author :    Michel Bierlaire and Mamy Fetiarison
// Date :      Tue Jun 16 12:47:18  2009
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sstream>

#include "bioArithSum.h"

/*!
*/
bioArithSum::bioArithSum(bioExpression* par,
                         bioExpression* left,
                         bioIteratorInfo * it) 
  : bioArithIterator(par,left,it)  {

}

bioArithSum::~bioArithSum() {}

patBoolean bioArithSum::isSum() const { 

  return patTRUE ;
}

patBoolean bioArithSum::isProd() const { 
  return patFALSE ;
}
