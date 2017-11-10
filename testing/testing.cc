//-*-c++-*------------------------------------------------------------
//
// File name : testing.cc
// Author :    Michel Bierlaire
// Date :      Sun Jul 12 16:54:24 2015
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "patDisplay.h"

#include "patGetWorkingDir.h"

int main(int argc, char *argv[]) {

  patGetWorkingDir g ;
  DEBUG_MESSAGE("Working directory: " << g()) ;
}
