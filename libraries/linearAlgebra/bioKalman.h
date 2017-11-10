//-*-c++-*------------------------------------------------------------
//
// File name : patKalman.h
// Author :   Michel Bierlaire
// Date :     Fri Jul 10 18:44:21 2015
//
//--------------------------------------------------------------------

#ifndef patKalman_h
#define patKalman_h

#include "patVariables.h"

class patKalman {

 public:
  patLu(patMyMatrix* aMat) ;
  void computeLu(patError*& err) ;
  const vector<unsigned long>* getPermutation() const ;
  const patVariables* solve(const patVariables& b,patError*& err) ;
  patBoolean isSuccessful() const ; 
 private:
  patMyMatrix* theMatrix ;
  vector<unsigned long> index ;
  short interchanges ;
  patBoolean success ;
  patBoolean luPerformed ;
  unsigned long n ;
  patVariables solution ;
};

#endif
