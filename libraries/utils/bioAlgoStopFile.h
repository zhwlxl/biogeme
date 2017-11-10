//-*-c++-*------------------------------------------------------------
//
// File name : bioAlgoStopFile.h
// Author :    Michel Bierlaire 
// Date :      Fri Aug  4 10:13:16 2017
//
//--------------------------------------------------------------------

#ifndef bioAlgoStopFile_h
#define bioAlgoStopFile_h

#include "bioAlgorithmManager.h"
#include "patString.h"

class bioAlgoStopFile : public bioAlgorithmManager {
 public:
  bioAlgoStopFile(patString f) ;
  patBoolean interruptIterations() ;
  patString reasonForInterruption() ;
 private:
  patString fileName ;

};
#endif
