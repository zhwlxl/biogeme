#ifndef YY_patBisonBeta_h_included
#define YY_patBisonBeta_h_included

#line 1 "/usr/local/lib/bison.h"
/* before anything */
#ifdef c_plusplus
#ifndef __cplusplus
#define __cplusplus
#endif
#endif
#ifdef __cplusplus
#ifndef YY_USE_CLASS
#define YY_USE_CLASS
#endif
#else
#endif
#include <stdio.h>

/* #line 14 "/usr/local/lib/bison.h" */
#line 21 "patBetaParser.h"
#define YY_patBisonBeta_ERROR_BODY  = 0
#define YY_patBisonBeta_LEX_BODY  = 0
#define YY_patBisonBeta_MEMBERS  patBetaScanner scanner; patParamManagement *pMaximizedFunction; patNL* nestedLogit ; patGenerateSimData::patDistributions* distrib ; virtual ~patBisonBeta() {};
#define YY_patBisonBeta_CONSTRUCTOR_PARAM  const string& fname_
#define YY_patBisonBeta_CONSTRUCTOR_INIT  : scanner(fname_) , pMaximizedFunction(NULL), nestedLogit(NULL), distrib(NULL)
#line 18 "patBetaParser.y"

  
#include <fstream.h>
#include <strstream.h>

#include "patDebug.h"
#include "patConst.h"
#include "patParamManagement.h"
#include "patGenerateSimData.h"
#include "patNL.h"

#undef yyFlexLexer
#define yyFlexLexer patBetaFlex
#include <FlexLexer.h>

class patBetaScanner : public patBetaFlex {

private:
                                    // filename to be scanned
  string _filename;

public:
                                    // void ctor
  patBetaScanner()
    : patBetaFlex() {
  }
                                    // ctor with filename argument
  patBetaScanner(const string& fname_)
    : patBetaFlex(), _filename( fname_ )  {
    DEBUG_MESSAGE("Opening " << fname_ << endl);
    ifstream *is = new ifstream( fname_.c_str() ); 
    if ( !is || (*is).fail() ) {
      cerr << "Error:: cannot open input file <";
      cerr << fname_ << ">" << endl;
      exit( 1 );
    }
    else {
      switch_streams( is, 0 );
    }
  }
                                    // dtor

  ~patBetaScanner() { delete yyin; }

                                    // utility functions

  const string& filename() const { return _filename; }

  string removeDelimeters( const string deli="\"\"" ) {
    
    string str = YYText();
    string::size_type carret = str.find("\n") ;
    if (carret < str.size()) str.patSTRINGERASE(carret) ;
    string::size_type deb = str.find( deli[0] ) ;
    if (deb == str.size()) {
      return ( str ) ;
    }
    str.patSTRINGERASE( deb , 1 );
    
    string::size_type fin = str.find( deli[1] ) ;
    if (fin >= str.size()) {
      WARNING("Unmatched delimiters (" << filename() << ":" << 
	      lineno() << ") ") ;
      return( str ) ;
    }
    str.patSTRINGERASE( fin , 1 );
    return ( str );
  }

  string value() {
    string str = YYText() ;
    return str; 
  }

  // char* value() { return (char*) YYText(); }

  void errorQuit( int err ) {
    cout << "Error = " << err << endl;
    if ( err == 0 ) return;
    cerr << "Problem in parsing"
	 << " (" << filename() << ":" << lineno() << ") "
	 << "Field: <" << YYText() << ">" << endl;
    if ( err < 0 ) exit( 1 );
  }
};




#line 110 "patBetaParser.y"
typedef union {
  short       itype;
  float       ftype;
  string*     stype;
  patGenerateSimData::patDistrib* dtype ;
} yy_patBisonBeta_stype;
#define YY_patBisonBeta_STYPE yy_patBisonBeta_stype

#line 14 "/usr/local/lib/bison.h"
 /* %{ and %header{ and %union, during decl */
#ifndef YY_patBisonBeta_COMPATIBILITY
#ifndef YY_USE_CLASS
#define  YY_patBisonBeta_COMPATIBILITY 1
#else
#define  YY_patBisonBeta_COMPATIBILITY 0
#endif
#endif

#if YY_patBisonBeta_COMPATIBILITY != 0
/* backward compatibility */
#ifdef YYLTYPE
#ifndef YY_patBisonBeta_LTYPE
#define YY_patBisonBeta_LTYPE YYLTYPE
/* WARNING obsolete !!! user defined YYLTYPE not reported into generated header */
/* use %define LTYPE */
#endif
#endif
#ifdef YYSTYPE
#ifndef YY_patBisonBeta_STYPE 
#define YY_patBisonBeta_STYPE YYSTYPE
/* WARNING obsolete !!! user defined YYSTYPE not reported into generated header */
/* use %define STYPE */
#endif
#endif
#ifdef YYDEBUG
#ifndef YY_patBisonBeta_DEBUG
#define  YY_patBisonBeta_DEBUG YYDEBUG
/* WARNING obsolete !!! user defined YYDEBUG not reported into generated header */
/* use %define DEBUG */
#endif
#endif
#ifdef YY_patBisonBeta_STYPE
#ifndef yystype
#define yystype YY_patBisonBeta_STYPE
#endif
#endif
/* use goto to be compatible */
#ifndef YY_patBisonBeta_USE_GOTO
#define YY_patBisonBeta_USE_GOTO 1
#endif
#endif

/* use no goto to be clean in C++ */
#ifndef YY_patBisonBeta_USE_GOTO
#define YY_patBisonBeta_USE_GOTO 0
#endif

#ifndef YY_patBisonBeta_PURE

/* #line 63 "/usr/local/lib/bison.h" */
#line 178 "patBetaParser.h"

#line 63 "/usr/local/lib/bison.h"
/* YY_patBisonBeta_PURE */
#endif

/* #line 65 "/usr/local/lib/bison.h" */
#line 185 "patBetaParser.h"

#line 65 "/usr/local/lib/bison.h"
/* prefix */
#ifndef YY_patBisonBeta_DEBUG

/* #line 67 "/usr/local/lib/bison.h" */
#line 192 "patBetaParser.h"

#line 67 "/usr/local/lib/bison.h"
/* YY_patBisonBeta_DEBUG */
#endif
#ifndef YY_patBisonBeta_LSP_NEEDED

/* #line 70 "/usr/local/lib/bison.h" */
#line 200 "patBetaParser.h"

#line 70 "/usr/local/lib/bison.h"
 /* YY_patBisonBeta_LSP_NEEDED*/
#endif
/* DEFAULT LTYPE*/
#ifdef YY_patBisonBeta_LSP_NEEDED
#ifndef YY_patBisonBeta_LTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YY_patBisonBeta_LTYPE yyltype
#endif
#endif
/* DEFAULT STYPE*/
#ifndef YY_patBisonBeta_STYPE
#define YY_patBisonBeta_STYPE int
#endif
/* DEFAULT MISCELANEOUS */
#ifndef YY_patBisonBeta_PARSE
#define YY_patBisonBeta_PARSE yyparse
#endif
#ifndef YY_patBisonBeta_LEX
#define YY_patBisonBeta_LEX yylex
#endif
#ifndef YY_patBisonBeta_LVAL
#define YY_patBisonBeta_LVAL yylval
#endif
#ifndef YY_patBisonBeta_LLOC
#define YY_patBisonBeta_LLOC yylloc
#endif
#ifndef YY_patBisonBeta_CHAR
#define YY_patBisonBeta_CHAR yychar
#endif
#ifndef YY_patBisonBeta_NERRS
#define YY_patBisonBeta_NERRS yynerrs
#endif
#ifndef YY_patBisonBeta_DEBUG_FLAG
#define YY_patBisonBeta_DEBUG_FLAG yydebug
#endif
#ifndef YY_patBisonBeta_ERROR
#define YY_patBisonBeta_ERROR yyerror
#endif

#ifndef YY_patBisonBeta_PARSE_PARAM
#ifndef __STDC__
#ifndef __cplusplus
#ifndef YY_USE_CLASS
#define YY_patBisonBeta_PARSE_PARAM
#ifndef YY_patBisonBeta_PARSE_PARAM_DEF
#define YY_patBisonBeta_PARSE_PARAM_DEF
#endif
#endif
#endif
#endif
#ifndef YY_patBisonBeta_PARSE_PARAM
#define YY_patBisonBeta_PARSE_PARAM void
#endif
#endif

/* TOKEN C */
#ifndef YY_USE_CLASS

#ifndef YY_patBisonBeta_PURE
extern YY_patBisonBeta_STYPE YY_patBisonBeta_LVAL;
#endif


/* #line 143 "/usr/local/lib/bison.h" */
#line 278 "patBetaParser.h"
#define	pat_gevBeta	258
#define	pat_gevMu	259
#define	pat_gevModel	260
#define	pat_gevScale	261
#define	pat_gevNests	262
#define	pat_gevAlt	263
#define	pat_Attr	264
#define	pat_UNIF	265
#define	pat_NORM	266
#define	patEQUAL	267
#define	patOB	268
#define	patCB	269
#define	patINT	270
#define	patREAL	271
#define	patTIME	272
#define	patNAME	273
#define	patSTRING	274
#define	patPAIR	275


#line 143 "/usr/local/lib/bison.h"
 /* #defines token */
/* after #define tokens, before const tokens S5*/
#else
#ifndef YY_patBisonBeta_CLASS
#define YY_patBisonBeta_CLASS patBisonBeta
#endif

#ifndef YY_patBisonBeta_INHERIT
#define YY_patBisonBeta_INHERIT
#endif
#ifndef YY_patBisonBeta_MEMBERS
#define YY_patBisonBeta_MEMBERS 
#endif
#ifndef YY_patBisonBeta_LEX_BODY
#define YY_patBisonBeta_LEX_BODY  
#endif
#ifndef YY_patBisonBeta_ERROR_BODY
#define YY_patBisonBeta_ERROR_BODY  
#endif
#ifndef YY_patBisonBeta_CONSTRUCTOR_PARAM
#define YY_patBisonBeta_CONSTRUCTOR_PARAM
#endif
/* choose between enum and const */
#ifndef YY_patBisonBeta_USE_CONST_TOKEN
#define YY_patBisonBeta_USE_CONST_TOKEN 0
/* yes enum is more compatible with flex,  */
/* so by default we use it */ 
#endif
#if YY_patBisonBeta_USE_CONST_TOKEN != 0
#ifndef YY_patBisonBeta_ENUM_TOKEN
#define YY_patBisonBeta_ENUM_TOKEN yy_patBisonBeta_enum_token
#endif
#endif

class YY_patBisonBeta_CLASS YY_patBisonBeta_INHERIT
{
public: 
#if YY_patBisonBeta_USE_CONST_TOKEN != 0
/* static const int token ... */

/* #line 182 "/usr/local/lib/bison.h" */
#line 341 "patBetaParser.h"
static const int pat_gevBeta;
static const int pat_gevMu;
static const int pat_gevModel;
static const int pat_gevScale;
static const int pat_gevNests;
static const int pat_gevAlt;
static const int pat_Attr;
static const int pat_UNIF;
static const int pat_NORM;
static const int patEQUAL;
static const int patOB;
static const int patCB;
static const int patINT;
static const int patREAL;
static const int patTIME;
static const int patNAME;
static const int patSTRING;
static const int patPAIR;


#line 182 "/usr/local/lib/bison.h"
 /* decl const */
#else
enum YY_patBisonBeta_ENUM_TOKEN { YY_patBisonBeta_NULL_TOKEN=0

/* #line 185 "/usr/local/lib/bison.h" */
#line 368 "patBetaParser.h"
	,pat_gevBeta=258
	,pat_gevMu=259
	,pat_gevModel=260
	,pat_gevScale=261
	,pat_gevNests=262
	,pat_gevAlt=263
	,pat_Attr=264
	,pat_UNIF=265
	,pat_NORM=266
	,patEQUAL=267
	,patOB=268
	,patCB=269
	,patINT=270
	,patREAL=271
	,patTIME=272
	,patNAME=273
	,patSTRING=274
	,patPAIR=275


#line 185 "/usr/local/lib/bison.h"
 /* enum token */
     }; /* end of enum declaration */
#endif
public:
 int YY_patBisonBeta_PARSE(YY_patBisonBeta_PARSE_PARAM);
 virtual void YY_patBisonBeta_ERROR(char *msg) YY_patBisonBeta_ERROR_BODY;
#ifdef YY_patBisonBeta_PURE
#ifdef YY_patBisonBeta_LSP_NEEDED
 virtual int  YY_patBisonBeta_LEX(YY_patBisonBeta_STYPE *YY_patBisonBeta_LVAL,YY_patBisonBeta_LTYPE *YY_patBisonBeta_LLOC) YY_patBisonBeta_LEX_BODY;
#else
 virtual int  YY_patBisonBeta_LEX(YY_patBisonBeta_STYPE *YY_patBisonBeta_LVAL) YY_patBisonBeta_LEX_BODY;
#endif
#else
 virtual int YY_patBisonBeta_LEX() YY_patBisonBeta_LEX_BODY;
 YY_patBisonBeta_STYPE YY_patBisonBeta_LVAL;
#ifdef YY_patBisonBeta_LSP_NEEDED
 YY_patBisonBeta_LTYPE YY_patBisonBeta_LLOC;
#endif
 int YY_patBisonBeta_NERRS;
 int YY_patBisonBeta_CHAR;
#endif
#if YY_patBisonBeta_DEBUG != 0
public:
 int YY_patBisonBeta_DEBUG_FLAG;	/*  nonzero means print parse trace	*/
#endif
public:
 YY_patBisonBeta_CLASS(YY_patBisonBeta_CONSTRUCTOR_PARAM);
public:
 YY_patBisonBeta_MEMBERS 
};
/* other declare folow */
#endif


#if YY_patBisonBeta_COMPATIBILITY != 0
/* backward compatibility */
#ifndef YYSTYPE
#define YYSTYPE YY_patBisonBeta_STYPE
#endif

#ifndef YYLTYPE
#define YYLTYPE YY_patBisonBeta_LTYPE
#endif
#ifndef YYDEBUG
#ifdef YY_patBisonBeta_DEBUG 
#define YYDEBUG YY_patBisonBeta_DEBUG
#endif
#endif

#endif
/* END */

/* #line 236 "/usr/local/lib/bison.h" */
#line 443 "patBetaParser.h"

#line 295 "patBetaParser.y"

//--------------------------------------------------------------------
// Following pieces of code will be verbosely copied into the parser.
//--------------------------------------------------------------------

class patBetaParser: public patBisonBeta {

public:
                                    // ctor with filename argument

  patBetaParser( const string& fname_ ) :	
    patBisonBeta( fname_.c_str() ) {}
  
                                    // dtor
  virtual ~patBetaParser () { }
                                    // Utility functions

  string filename() const { return scanner.filename(); }

  void yyerror( char* msg ) {
    cout << "Call to yyerror" << endl << endl ;
    cerr << *msg;
    cerr << " (" << filename() << ":" << scanner.lineno() << ") ";
    cerr << "Field: <" << scanner.YYText() << ">" << endl;
    exit( 1 );
  }

  int yylex() { return scanner.yylex(); }

  patBoolean parse( patParamManagement *p ,
		    patNL* nl ,
		    patGenerateSimData::patDistributions* d) {
    if ( (p && pMaximizedFunction) || (nl && nestedLogit) || (d && distrib) ) {
      cerr << "\nError:: cannot parse <";
      cerr << filename() << "> twice" << endl;
      return( patFALSE );
    }
    else {
      ostrstream os;
      os << "Parsing <"
	 << filename() << ">";
      nestedLogit = nl ;
      pMaximizedFunction = p;
      distrib = d ;
      yyparse();
      return(patTRUE);
    }
  }
};

 #endif
