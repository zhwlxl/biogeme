//-*-c++-*------------------------------------------------------------
//
// File name : patSymmZeroDiagMatrix.h
// Author :    \URL[Michel Bierlaire]{http://rosowww.epfl.ch/mbi}
// Date :      Fri Feb  5 13:53:59 1999
//
//--------------------------------------------------------------------

#ifndef patSymmZeroDiagMatrix_h
#define patSymmZeroDiagMatrix_h

/**
@doc  This class implements a symmetric matrix within a vector. It stores only
 once each off diagonal elements to spare storage.
 Warning: diagonal elements are assumed to be zero.
 @author \URL[Michel Bierlaire]{http://rosowww.epfl.ch/mbi}, EPFL (Fri Feb  5 13:53:59 1999)
*/
template <class T>
class patSymmZeroDiagMatrix {

public :

  patSymmZeroDiagMatrix(unsigned long size = 0) : data(1+size*(size-1)/2) {
    DEBUG_MESSAGE("Size = " << size) ;
    DEBUG_MESSAGE("Entries = " << size*(size-1)/2) ;
  } ;
  virtual ~patSymmZeroDiagMatrix() {
    data.erase(data.begin(),data.end()) ;
  }
  void setElement(vector<T>::size_type i, vector<T>::size_type j, const T& x) {
    if (i == j) {
      WARNING("Diagonal must be zero") ;
      return ;
    }
    if (index(i,j) >= data.size()) {
      WARNING("index(" << i << "," << j << ")=" << index(i,j) << " size=" << data.size()) ;
      exit(-1) ;
    }
    data[index(i,j)] = x ;
  } 
  T getElement(vector<T>::size_type i, vector<T>::size_type j) const {
    if (i == j) return T(0) ;
    if (index(i,j) >= data.size()) {
      WARNING("index("<<i<< "," << j << ")=" << index(i,j) << " size=" << data.size()) ;
      exit(-1) ;
    }
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
	      ? 1+j+(i*(i-1)/2) : 1+i+(j*(j-1)/2)) ;
    }
};

#endif
