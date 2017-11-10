//-*-c++-*------------------------------------------------------------
//
// File name : patFileTime.cc
// Author :    Michel Bierlaire
// Date :      Fri Aug  4 09:32:43 2017
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_STAT
#include <sys/stat.h>
#endif

#include "patFileTime.h"
#include "patAbsTime.h"


patFileTime::patFileTime(patString f) : fileName(f),calculated(patFALSE)  {

}

patBoolean patFileTime::calculateModificationTime() {
#ifdef HAVE_STAT
  struct stat t_stat;
  stat(fileName.c_str(), &t_stat);
  struct tm * timeinfo = localtime(&t_stat.st_ctime); 
  theTime.setTime(*timeinfo) ;
  return patTRUE ;
#else
  return patFALSE ;
#endif
}

std::ostream& operator<<(std::ostream &str, patFileTime& x) {
  if (x.calculateModificationTime()) {
    str << "File " << x.fileName << " modified on " << x.theTime.getTimeString(patTsfFULL) ;
  }
  else {
    str << "File " << x.fileName << ": unknown modiciation time" ;
  }
  return str ;
  
}

patBoolean operator<(patFileTime& t1, patFileTime& t2) {
  if (t1.calculateModificationTime()) {
    if (t2.calculateModificationTime()) {
      return (t1.theTime < t2.theTime) ;
    }
  }
  return patFALSE ;
}
