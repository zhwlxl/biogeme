//-*-c++-*------------------------------------------------------------
//
// File name : bioRInterface.cc
// Author :    Michel Bierlaire and Nikola Obrenovic
// Date :      Sun Oct 22 16:44:26 2017
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <iostream>
#include "bioRInterface.h"

extern "C" SEXP biogeme(SEXP ptrDataFrame) {
    DataFrame df = Rcpp::as<DataFrame>(ptrDataFrame);
    int nCols=df.ncol();
    int nRows=df.nrow();
    std::cout << "Number of rows:    " << nRows << std::endl ;
    std::cout << "Number of columns: " << nCols << std::endl ;
    CharacterVector theColNames = df.names() ;
    for (int i = 0 ; i < nCols ; ++i) {
      std::cout << "Name of column " << i <<": " << theColNames[i] << std::endl ;
    }
}
