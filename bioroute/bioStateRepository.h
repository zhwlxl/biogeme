//-*-c++-*------------------------------------------------------------
//
// File name : bioStateRepository.h
// Author :    Michel Bierlaire 
// Date :      Sun Aug 29 10:50:58 2010
//
//--------------------------------------------------------------------

#ifndef bioStateRepository_h
#define bioStateRepository_h

#include "bioMhPathGenState.h"

class bioStateRepository {

 public:
  static bioStateRepository* the() ;
  patULong addState(bioMhPathGenState aState, patError*& err) ;
  patULong getId(bioMhPathGenState aState, patError*& err) ;
  void print(patString fileName) ;
 private:
  patULong currentId ;
  bioStateRepository() ;
  map<bioMhPathGenState,patULong> theIds ;
};
#endif
