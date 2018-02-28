//-*-c++-*------------------------------------------------------------
//
// File name : biogemedylib.cc
// Author :    Michel Bierlaire
// Date :      Tue Dec  5 18:32:08 2017
//
//--------------------------------------------------------------------

#include <iostream>
#include "patType.h"
#include "bioMain.h"
#include "patError.h"
#include "patDisplay.h"
#include "patEnvPathVariable.h"

extern "C" {

  void addPathToPythonPath(char* path) {
   patEnvPathVariable pythonPath("PYTHONPATH") ;
   pythonPath.readFromSystem() ;
   patString thePath(path) ;
   pythonPath.addPath(thePath) ;
   patError* err(NULL) ;
   pythonPath.registerToSystem(err) ;
   if (err != NULL) {
     WARNING(err->describe());
     return ;
   }
   
  }
  
  void runBiogeme(char* pyfile, char* datfile) {
    patString modelFileName(pyfile) ;
    patString dataFileName(datfile) ;
    patError* err(NULL) ;

    bioMain theMain ;

    DEBUG_MESSAGE("DLL: run pythonbiogeme " << modelFileName << " " << dataFileName)
    theMain.run(modelFileName,dataFileName,err) ;
    if (err != NULL) {
      WARNING(err->describe());
      return ;
    }
  }
}

