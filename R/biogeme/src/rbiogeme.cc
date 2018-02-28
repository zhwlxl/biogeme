#include <Rcpp.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <iostream>
#include <fstream>

using namespace std;
using namespace Rcpp;


void runBiogeme(SEXP p_df)
{
    //Cast input SEXP object into Rcpp data.frame representation.
    DataFrame df = Rcpp::as<DataFrame>(p_df);

    string modelFileName = "modFile.mod";
    string dataFileName = "dataFile.dat";

    ofstream modelFile;
    modelFile.open(modelFileName);
    string text = "Write this to the file";
    modelFile << text;
    modelFile.close();


    ofstream dataFile;
    dataFile.open(dataFileName);
    modelFile << "Write this to the file";
    modelFile.close();

    //Biogeme bio = new Biogeme();
    //bio.run(modelFileName, dataFileName);

}

static const R_CallMethodDef CallEntries[] = {
  {"runBiogeme", (DL_FUNC) &runBiogeme, 1},
  {NULL, NULL, 0}
};

RcppExport void R_init_Biogeme3(DllInfo *dll) {
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, TRUE);
};
