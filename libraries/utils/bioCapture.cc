//-*-c++-*------------------------------------------------------------
//
// File name : bioCapture.cc
// Author :    Michel Bierlaire,
//             based on a post by Björn Pollex on stackoverflow
// Date :      Mon Feb 12 16:28:52 2018
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "bioCapture.h"

bioCapture::bioCapture(): oldOutBuffer(NULL), oldErrBuffer(NULL) {
  beginCapture() ;
}

bioCapture::~bioCapture() {
  endCapture() ;
}

void bioCapture::beginCapture() {
  beginCaptureStdout() ;
  beginCaptureStderr() ;
}

void bioCapture::beginCaptureStdout() {
  oldOutBuffer = std::cout.rdbuf(stringOutBuffer.rdbuf());
}
void bioCapture::beginCaptureStderr() {
  oldErrBuffer = std::cerr.rdbuf(stringErrBuffer.rdbuf());
}

void bioCapture::endCapture() {
  endCaptureStdout() ;
  endCaptureStderr() ;
}

void bioCapture::endCaptureStdout() {
  std::cout.rdbuf( oldOutBuffer );
  oldOutBuffer = NULL ;
  stringOutBuffer.flush() ;
}

void bioCapture::endCaptureStderr() {
  std::cerr.rdbuf( oldErrBuffer );
  oldErrBuffer = NULL ;
  stringErrBuffer.flush() ;
}

patString bioCapture::getCaptureStdout() {
  return stringOutBuffer.str();
}

patString bioCapture::getCaptureStderr() {
  return stringErrBuffer.str();
}
