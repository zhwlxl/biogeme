//-*-c++-*------------------------------------------------------------
//
// File name : bioStatistics.cc
// Author :    Michel Bierlaire and Mamy Fetiarison
// Date :      Sat Dec 19 19:25:35 2009
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "bioStatistics.h"

vector<pair<patString,patReal> >* bioStatistics::getStatistics() {
  return &statistics ;
}
