//-*-c++-*------------------------------------------------------------
//
// File name : patGetWorkingDir.h
// Author :    Michel Bierlaire
// Date :      Sat Apr  1 19:10:48 2017
//
//--------------------------------------------------------------------

#ifndef patGetWorkingDir_h
#define patGetWorkingDir_h

#include <cstdio>

#include "patString.h"

class patGetWorkingDir {
 public:
  patGetWorkingDir() ;
  patString operator()() ;
  char buffer[FILENAME_MAX] ;
};
#endif
