//-*-c++-*------------------------------------------------------------
//
// File name : patAdVariables.h
// Author :    \URL[Michel Bierlaire]{http://rosowww.epfl.ch/mbi}
// Date :      Wed Aug 30 17:38:04 2000
//
// Version of patVariables for automatic differentiation
//
//--------------------------------------------------------------------

#ifndef patAdVariables_h
#define patAdVariables_h

#ifdef ADOLC

#include "patAd.h"
#include <vector>
#include "patType.h"
#include <iostream>

/** @doc Array for automatic differentiation
 */

typedef adoublev patAdVariables;

#endif
#endif


