//-*-c++-*------------------------------------------------------------
//
// File name : bioGslParams.h
// Author :    Michel Bierlaire
// Date :      Wed Mar 31 17:37:08 2010
//
//--------------------------------------------------------------------

#ifndef bioGslParams_h
#define bioGslParams_h

//#include <gsl/gsl_errno.h>
//#include <gsl/gsl_integration.h>

/*!
  Structure to store the parameters to be passed to a GSL function.
*/

class bioSample ;

#include "patVariables.h"
#include "trVector.h"
#include "patMath.h"
#include "bioIteratorSpan.h"

struct bioGslParams {
  patVariables* __x ;
  vector< vector<patReal> >* __draw ;
  trVector* __beta ;
  bioSample* sample ;
  bioIteratorSpan __theThreadSpan ;
  bioIteratorSpan __theCurrentSpan ;
};



#endif
