//-*-c++-*------------------------------------------------------------
//
// File name : patPointer.h
// Author :    \URL[Michel Bierlaire]{http://rosowww.epfl.ch/mbi}
// Date :      Wed Mar  3 20:32:11 1999
//
//--------------------------------------------------------------------


#ifndef patPointer_h
#define patPointer_h

#include <assert.h>

/**
  @doc  This class encapsulates pointers into object. This is specifically done to accomodate some requirements from STL 
   @author \URL[Michel Bierlaire]{http://rosowww.epfl.ch/mbi}, EPFL (Wed Mar  3 20:32:11 1999)
 */

template <class T>
class patPointer {
public:
  /**
   */
  patPointer(T* p = NULL) : ptr(p) {} ;
  /**
   */
  T* operator()() const { return ptr ;}
  /**
   */
  T* operator->() const { return ptr ;}
  /**
   */
  void releaseMemory() {if (ptr != NULL) delete ptr ; ptr = NULL ; }
private:
  T* ptr ;

};

#endif 
