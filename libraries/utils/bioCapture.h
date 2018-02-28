//-*-c++-*------------------------------------------------------------
//
// File name : bioCapture.h
// Author :    Michel Bierlaire,
//             based on a post by Björn Pollex on stackoverflow
// Date :      Mon Feb 12 16:18:27 2018
//
//--------------------------------------------------------------------

#ifndef bioCapture_h
#define bioCapture_h

#include <iostream>
#include <strstream>
#include "patString.h"

class bioCapture {
  friend class patSingletonFactory ;
 public:
  bioCapture() ;
  virtual ~bioCapture() ;
  patString getCaptureStderr() ;
  patString getCaptureStdout() ;
  void beginCapture() ;
  void endCapture() ;
  void beginCaptureStderr() ;
  void endCaptureStderr() ;
  void beginCaptureStdout() ;
  void endCaptureStdout() ;
 private:
  std::streambuf * oldErrBuffer ;
  std::streambuf * oldOutBuffer ;
  std::stringstream stringErrBuffer ;
  std::stringstream stringOutBuffer ;
};

#endif
