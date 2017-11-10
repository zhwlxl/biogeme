//-*-c++-*------------------------------------------------------------
//
// File name : patRandomInteger.cc
// Author :    Michel Bierlaire
// Date :      Thu Aug 12 11:44:30 2010
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "patDisplay.h"
#include "patRandomInteger.h"
#include "patSingletonFactory.h"

patRandomInteger* patRandomInteger::the() {
  return patSingletonFactory::the()->patRandomInteger_the() ;
}

patRandomInteger::patRandomInteger(patULong seed) {
  srand(seed) ;
}

patULong patRandomInteger::drawNumber(patULong minNumber, patULong maxNumber) {
    patULong result = (minNumber + rand() % (maxNumber - minNumber)) ;
    return result ;
}

