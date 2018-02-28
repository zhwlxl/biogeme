#include <Rcpp.h>
#include <OtherLibDyn.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <iostream>
#include <fstream>

using namespace std;
using namespace Rcpp;

//This function version is not explicitly linking the other library
RcppExport SEXP calcStats(SEXP p_df)
{
  //Cast input SEXP object into Rcpp data.frame representation.
  DataFrame df = Rcpp::as<DataFrame>(p_df);

  double min = 1000000, max = 0, sum = 0;

  //Get column "salary" from the data frame.
  NumericVector nv = df["salary"];

  //nv.size() gives the length of the column.
  for(int i = 0;i<nv.size();i++)
  {
    if(nv[i] < min) min = nv[i];
    else if(nv[i] > max) max = nv[i];

    sum += nv[i];
  }

  //Create Rcpp vector of reals.
  //Named("min",min), Named("max")=max and _["sum"]=sum are different ways of adding a named element to the vector.
  //I haven't found if there are functional differences between these three.
  NumericVector v = NumericVector::create(Named("min",min), Named("max")=max , _["sum"]=sum, _["test"]=MakeNumDyn());

  return v;
}

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
  {"calcStats", (DL_FUNC) &calcStats, 1},
  {"runBiogeme", (DL_FUNC) &runBiogeme, 1},
  {NULL, NULL, 0}
};

RcppExport void R_init_Biogeme3(DllInfo *dll) {
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, TRUE);
};
