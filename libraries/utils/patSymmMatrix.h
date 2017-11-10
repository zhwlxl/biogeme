//-*-c++-*------------------------------------------------------------
//
// File name : patSymmMatrix.h
// Author :    \URL[Michel Bierlaire]{http://rosowww.epfl.ch/mbi}
// Date :      Fri May 28 11:33:43 1999
//
//--------------------------------------------------------------------

#ifndef patSymmMatrix_h
#define patSymmMatrix_h

#include "patDisplay.h"
#include "patType.h"
#include "patError.h"
#include "patVariables.h"

/**
@doc This class implements a symmetric matrix within a vector. It stores only
 once each off diagonal elements to spare storage.
 @author \URL[Michel Bierlaire]{http://rosowww.epfl.ch/mbi}, EPFL (Fri May 28 11:33:43 1999)
*/

template <class T>
class patSymmMatrix {

public :

  patSymmMatrix(vector<T>::size_type size = 0) : data(size*(size+1)/2) {
    DEBUG_MESSAGE("Size = " << size) ;
    DEBUG_MESSAGE("Entries = " << size*(size+1)/2) ;
  } ;
  virtual ~patSymmMatrix() {
    data.erase(data.begin(),data.end()) ;
  }
  void setElement(vector<T>::size_type i, vector<T>::size_type j, const T& x) {
    if (index(i,j) >= data.size()) {
      WARNING("index(" << i << "," << j << ")=" << index(i,j) << " size=" << data.size()) ;
      exit(-1) ;
    }
    assert(index(i,j) >= 0) ;
    data[index(i,j)] = x ;
  } 
  T getElement(vector<T>::size_type i, vector<T>::size_type j) const {
    if (index(i,j) >= data.size()) {
      WARNING("index("<<i<< "," << j << ")=" << index(i,j) << " size=" << data.size()) ;
      exit(-1) ;
    }
    assert(index(i,j) >= 0) ;
    return data[index(i,j)] ;
  }
  T operator()(vector<T>::size_type i,vector<T>::size_type j) const {
    return getElement(i,j) ;
  }

private:
  
  vector<T> data ;
  vector<T>::size_type index(vector<T>::size_type i,
			     vector<T>::size_type j) const { 
    return ((i>j) 
	    ? j+(i*(i+1)/2) : i+(j*(j+1)/2)) ;
  }
  
};

#endif
