//-*-c++-*------------------------------------------------------------
//
// File name : bioAlgoStopFile.cc
// Author :    Michel Bierlaire 
// Date :      Fri Aug  4 10:17:40 2017
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "bioAlgoStopFile.h"
#include "patFileExists.h"

bioAlgoStopFile::bioAlgoStopFile(patString f) : fileName(f) {

}

patBoolean bioAlgoStopFile::interruptIterations() {
  return patFileExists()(fileName) ;
}

patString bioAlgoStopFile::reasonForInterruption() {
  stringstream str ;
  str <<"File " << fileName << " exists" ;
  return patString(str.str()) ;
}
