//-*-c++-*------------------------------------------------------------
//
// File name : patMyString.h
// Author :    \URL[Michel Bierlaire]{http://roso.epfl.ch/mbi}
// Date :      Sun Jun 12 20:23:46 2005
//
//--------------------------------------------------------------------

#ifndef patMyString_h
#define patMyString_h

#include <vector>
#include <string>
using namespace std ;

class patMyString {

  typedef size_type vector<char>::size_type ;
  
 public:
  /**
   */
  patMyString() ;

  /**
   */
  patMyString(const char[]) ;

  /**
   */
  patMyString(const string&) ;

  /**
   */
  const char* c_str() const ;

  /**
   */
  patMyString substr(size_type pos, size_type len) const ;

  /**
   */
  patMyString& operator+=(const patMyString& y) ;

  /**
   */
  size_type size() const ;

  /**
   */
  const char& operator[](size_type pos) const ;
  /**
   */
  char& operator[](size_type pos)  ;

  /**
   */
  size_type find(const patMyString& str, 
		 size_type pos = 0) ;

  /**
   */
  patMyString& replace(size_type pos,
		       size_type n1,
		       const patMyString&) ;
const
 protected:
  /**
   */
  vector<char> theString ;
} ;

/**
 */
ostream& operator<<(ostream &str, const patMyString& x) ;


#endif
