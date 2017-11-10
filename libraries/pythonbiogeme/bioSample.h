//-*-c++-*------------------------------------------------------------
//
// File name : bioSample.h
// Author :    Michel Bierlaire and Mamy Fetiarison
// Date :      Fri Jun 19 10:05:49  2009
//
//--------------------------------------------------------------------

#ifndef bioSample_h
#define bioSample_h

#include <vector>
#include <map>
#include "patError.h"
#include "patTimeInterval.h"
#include "patVariables.h"
#include "bioIteratorSpan.h"
class bioMetaIterator ;
class bioRandomMetaIterator ;
class bioRowIterator ;
class bioIdIterator ;
class bioExpression ;
/*!

*/



/*!
   This object is in charge of the sample management.
 */
class bioSample {

public : //Methods

  bioSample(patString filename, patError*& err) ;

  ~bioSample() ;

  patULong size() const ;

  vector<patString> getHeaders() const ;

  patVariables* at(patULong index) ;

  patULong getIndexFromHeader(patString h, patError*& err) const ;

  patBoolean isEqual(bioSample s) ;
  
  void printSample() ;

  patULong getNumberOfColumns() const ;

  bioMetaIterator* createMetaIterator(bioIteratorSpan theSpan, bioIteratorSpan theThreadSpan, patError*& err) const ;

  // For bootstrapping on panel data
  bioRandomMetaIterator* createRandomMetaIterator(bioIteratorSpan theSpan, bioIteratorSpan theThreadSpan, patError*& err) const ;

  bioRowIterator* createRowIterator(bioIteratorSpan theSpan, bioIteratorSpan theThreadSpan, patBoolean printMessages, patError*& err) const ;

  bioRowIterator* createIdIterator(bioIteratorSpan theSpan, bioIteratorSpan theThreadSpan, patString header, patError*& err) const ;

  void prepare(bioExpression* exclude,
	       vector<pair<patString, bioExpression*> >* userExpressions, 
	       patError*& err) ;

  vector<patULongLong> getListOfIds(patString header, patError*& err) ;

  // Assume that there are N individuals in the sample, identified by
  // the third argument. Then the vector of values must contain N
  // values.  Value #n is associated with the first observation of
  // individual n if duplicate is patFALSE, and to all observations if
  // duplicate is patTRUE. The getcolumn function is based on the same
  // conventions.
  void setColumn(patString column, vector<patReal> values, patBoolean duplicate, patString individual,patError*& err) ;
  vector<patReal> getColumn(patString column, patString individual, patError*& err) ;

  patULong getNbrExcludedRow() const ;
  const patVariables* getFirstRow() ;

  void processRow(bioExpression* exclude, 
		  vector<patReal> listOfV, 
		  vector<pair<patString, bioExpression*> >* userExpressions, 
		  patError*& err) ;

  patString getProcessingTime() const ;

  // Name of the data file used for estimation. It is either the text
  // file or the binary file
  patString getDataFileName() const ;

  bioSample getBootstrap(patError*& err) ;
  
private: // Methods
  bioSample getRegularBootstrap(patError*& err) ;
  bioSample getPanelBootstrap(patError*& err) ;
  bioSample(patULong _nbrOfExcludedRows,
	    patReal _rowNumber,
	    patULong _physicalRowNumber,
	    vector<patString>& _headers,
	    vector<patVariables>& _samples,
	    patULong _numberOfRealColumns,
	    patString _fileName) ;
  patString getTextFileName() const ;
  patString getBinaryFileName() const ;
  void computeMapOfDatabase(patError*& err) ;
  void generatePythonHeaders(patError*& err) ;
  void readFile(bioExpression* exclude, 
		vector<pair<patString, bioExpression*> >* userExpressions, 
		patError*& err) ;

  void readTextFile(bioExpression* exclude, 
		vector<pair<patString, bioExpression*> >* userExpressions, 
		patError*& err) ;
  void readBinaryFile(bioExpression* exclude, 
		vector<pair<patString, bioExpression*> >* userExpressions, 
		patError*& err) ;
  void addObservationData(patVariables data) ;
  void printHeaders() ;


private : // Data

  patULong nbrOfExcludedRows ;
  patReal rowNumber ;
  patULong physicalRowNumber ;
  vector<patString> headers ;
  vector<patVariables> samples ;
  patULong numberOfRealColumns ;
  patString fileName ;
  patTimeInterval processTime ;
  patBoolean dataReadFromBinary ;

};

#endif
