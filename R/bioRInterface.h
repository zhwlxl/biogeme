//-*-c++-*------------------------------------------------------------
//
// File name : bioRInterface.h
// Author :    Michel Bierlaire and Nikola Obrenovic
// Date :      Sun Oct 22 16:40:46 2017
//
//--------------------------------------------------------------------

#ifndef bioRInterface_h
#define bioRInterface_h 

/**
@doc This defines an interface between R and Biogeme

*/

#include "Rcpp.h"

using namespace Rcpp;

extern "C" SEXP biogeme(SEXP ptrDataFrame);



#endif
