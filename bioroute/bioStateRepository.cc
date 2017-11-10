//-*-c++-*------------------------------------------------------------
//
// File name : bioStateRepository.cc
// Author :    Michel Bierlaire 
// Date :      Sun Aug 29 10:55:34 2010
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "bioStateRepository.h"

bioStateRepository::bioStateRepository() : currentId(0) {

}

bioStateRepository* bioStateRepository::the() {
  static bioStateRepository* singleInstance = NULL ;
  if (singleInstance == NULL) {
    singleInstance = new bioStateRepository ;
  }
  return singleInstance ;
}

patULong bioStateRepository::addState(bioMhPathGenState aState, patError*& err) {
  map<bioMhPathGenState,patULong>::iterator found = theIds.find(aState) ;
  if (found == theIds.end()) {
    theIds[aState] = currentId ;
    ++currentId ;
    return (currentId-1) ;
  }
  return (found->second) ;
}

patULong bioStateRepository::getId(bioMhPathGenState aState, patError*& err) {
  map<bioMhPathGenState,patULong>::iterator found = theIds.find(aState) ;
  if (found == theIds.end()) {
    return (patBadId) ;
  }
  return (found->second) ;
  
}


void bioStateRepository::print(patString fileName) {
  ofstream f(fileName.c_str()) ;
  for (map<bioMhPathGenState,patULong>::iterator i = theIds.begin() ;
       i != theIds.end() ;
       ++i) {
    f << "[" << i->second << "] " << i->first << endl ;
  }
  f.close() ;
}

