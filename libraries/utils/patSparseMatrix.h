//-*-c++-*------------------------------------------------------------
//
// File name : patSparseMatrix.h
// Author :    \URL[Michel Bierlaire]{http://rosowww.epfl.ch/mbi}
// Date :      Fri Jun  2 15:38:53 2000
//
//--------------------------------------------------------------------

#include <utility>
#include <vector>
#include "sp/coord_double.h"

/**
@doc This class should be used to prepare sparse matrices to be used in the
 SparseLib++ library, by R. Pozo. It is not designed for intensive
 computation. Use sparselib++ instead.
 @author \URL[Michel Bierlaire]{http://rosowww.epfl.ch/mbi}, EPFL (Fri Jun  2 15:38:53 2000) 
*/

class patSparseMatrix {

public :
  /**
   */
  patSparseMatrix() ; 
  /**
   */
  patSparseMatrix(int m, int n) ;
  /**
   */
  void addElement(int i,int j,patReal elem) ;
  /**
   */
  Coord_Mat_double getSparseLibCoordMatrix()  ;
  /**
   */
  pair<int,int> getSize() const ;
private :
  int M ;
  int N ;
  vector<patReal> values ;
  vector<int> row ;
  vector<int> column ;
};

