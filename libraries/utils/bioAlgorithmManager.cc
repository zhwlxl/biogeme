//-*-c++-*------------------------------------------------------------
//
// File name : bioAlgorithmManager.cc
// Author :    Michel Bierlaire 
// Date :      Fri Aug  4 10:15:35 2017
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "bioAlgorithmManager.h"

bioAlgorithmManager::~bioAlgorithmManager() {

}

void bioAlgorithmManager::setCurrentIterate(const patVariables& _x) {
  x = _x ;
}

void bioAlgorithmManager::setCurrentFunction(patReal _f) {
  f = _f ;
}

void bioAlgorithmManager::setCurrentGradient(const patVariables& _g) {
  g = _g ;
}
