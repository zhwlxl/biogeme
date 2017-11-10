//-*-c++-*------------------------------------------------------------
//
// File name : patRandomInteger.h
// Author :    Michel Bierlaire
// Date :      Thu Aug 12 09:45:40 2010
//
//--------------------------------------------------------------------

#ifndef patRandomInteger_h
#define patRandomInteger_h

#include <cstdlib>
#include "patType.h"

class patRandomInteger {
    friend class patSingletonFactory ;

 public:
  static patRandomInteger* the() ;
  // Draw a random integer between minNumber and maxNumber-1, with equal probability
  patULong drawNumber(patULong minNumber, patULong maxNumber) ;
private:
  patRandomInteger(patULong seed = 9021967) ;

};
#endif
