//-*-c++-*------------------------------------------------------------
//
// File name : patFileTime.h
// Author :    Michel Bierlaire
// Date :      Fri Aug  4 09:31:06 2017
//
//--------------------------------------------------------------------

// Class to display the modification time of a file 

#ifndef patFileTime_h
#define patFileTime_h

#include "patString.h"
#include "patAbsTime.h"


class patFileTime {
  friend std::ostream& operator<<(std::ostream &str, patFileTime& x) ;
friend patBoolean operator<(patFileTime& t1, patFileTime& t2) ;
 public:
  patFileTime(patString f) ;
  patBoolean calculateModificationTime() ;
 private:
  patString fileName ;
  patAbsTime theTime ;
  patBoolean calculated ;
  
};
#endif
