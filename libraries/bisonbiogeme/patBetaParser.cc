#define YY_patBisonBeta_h_included

/*  A Bison++ parser, made from patBetaParser.y  */

 /* with Bison++ version bison++ Version 1.21-8, adapted from GNU bison by coetmeur@icdc.fr
  */


#line 1 "/usr/local/lib/bison.cc"
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Bob Corbett and Richard Stallman

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 1, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* HEADER SECTION */
#if defined( _MSDOS ) || defined(MSDOS) || defined(__MSDOS__) 
#define __MSDOS_AND_ALIKE
#endif
#if defined(_WINDOWS) && defined(_MSC_VER)
#define __HAVE_NO_ALLOCA
#define __MSDOS_AND_ALIKE
#endif

#ifndef alloca
#if defined( __GNUC__)
#define alloca __builtin_alloca

#elif (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc)  || defined (__sgi)
#include <alloca.h>

#elif defined (__MSDOS_AND_ALIKE)
#include <malloc.h>
#ifndef __TURBOC__
/* MS C runtime lib */
#define alloca _alloca
#endif

#elif defined(_AIX)
#include <malloc.h>
#pragma alloca

#elif defined(__hpux)
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */

#endif /* not _AIX  not MSDOS, or __TURBOC__ or _AIX, not sparc.  */
#endif /* alloca not defined.  */
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
#ifndef __STDC__
#define const
#endif
#endif
#include <stdio.h>
#define YYBISON 1  

/* #line 73 "/usr/local/lib/bison.cc" */
#line 85 "patBetaParser.cc"
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

#line 73 "/usr/local/lib/bison.cc"
/* %{ and %header{ and %union, during decl */
#define YY_patBisonBeta_BISON 1
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
#endif
#endif
#ifdef YYSTYPE
#ifndef YY_patBisonBeta_STYPE 
#define YY_patBisonBeta_STYPE YYSTYPE
#endif
#endif
#ifdef YYDEBUG
#ifndef YY_patBisonBeta_DEBUG
#define  YY_patBisonBeta_DEBUG YYDEBUG
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

/* #line 117 "/usr/local/lib/bison.cc" */
#line 237 "patBetaParser.cc"

#line 117 "/usr/local/lib/bison.cc"
/*  YY_patBisonBeta_PURE */
#endif

/* section apres lecture def, avant lecture grammaire S2 */

/* #line 121 "/usr/local/lib/bison.cc" */
#line 246 "patBetaParser.cc"

#line 121 "/usr/local/lib/bison.cc"
/* prefix */
#ifndef YY_patBisonBeta_DEBUG

/* #line 123 "/usr/local/lib/bison.cc" */
#line 253 "patBetaParser.cc"

#line 123 "/usr/local/lib/bison.cc"
/* YY_patBisonBeta_DEBUG */
#endif


#ifndef YY_patBisonBeta_LSP_NEEDED

/* #line 128 "/usr/local/lib/bison.cc" */
#line 263 "patBetaParser.cc"

#line 128 "/usr/local/lib/bison.cc"
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
      /* We used to use `unsigned long' as YY_patBisonBeta_STYPE on MSDOS,
	 but it seems better to be consistent.
	 Most programs should declare their own type anyway.  */

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
#if YY_patBisonBeta_COMPATIBILITY != 0
/* backward compatibility */
#ifdef YY_patBisonBeta_LTYPE
#ifndef YYLTYPE
#define YYLTYPE YY_patBisonBeta_LTYPE
#else
/* WARNING obsolete !!! user defined YYLTYPE not reported into generated header */
#endif
#endif
#ifndef YYSTYPE
#define YYSTYPE YY_patBisonBeta_STYPE
#else
/* WARNING obsolete !!! user defined YYSTYPE not reported into generated header */
#endif
#ifdef YY_patBisonBeta_PURE
#ifndef YYPURE
#define YYPURE YY_patBisonBeta_PURE
#endif
#endif
#ifdef YY_patBisonBeta_DEBUG
#ifndef YYDEBUG
#define YYDEBUG YY_patBisonBeta_DEBUG 
#endif
#endif
#ifndef YY_patBisonBeta_ERROR_VERBOSE
#ifdef YYERROR_VERBOSE
#define YY_patBisonBeta_ERROR_VERBOSE YYERROR_VERBOSE
#endif
#endif
#ifndef YY_patBisonBeta_LSP_NEEDED
#ifdef YYLSP_NEEDED
#define YY_patBisonBeta_LSP_NEEDED YYLSP_NEEDED
#endif
#endif
#endif
#ifndef YY_USE_CLASS
/* TOKEN C */

/* #line 236 "/usr/local/lib/bison.cc" */
#line 376 "patBetaParser.cc"
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


#line 236 "/usr/local/lib/bison.cc"
 /* #defines tokens */
#else
/* CLASS */
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
#ifndef YY_patBisonBeta_CONSTRUCTOR_CODE
#define YY_patBisonBeta_CONSTRUCTOR_CODE
#endif
#ifndef YY_patBisonBeta_CONSTRUCTOR_INIT
#define YY_patBisonBeta_CONSTRUCTOR_INIT
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

/* #line 280 "/usr/local/lib/bison.cc" */
#line 444 "patBetaParser.cc"
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


#line 280 "/usr/local/lib/bison.cc"
 /* decl const */
#else
enum YY_patBisonBeta_ENUM_TOKEN { YY_patBisonBeta_NULL_TOKEN=0

/* #line 283 "/usr/local/lib/bison.cc" */
#line 471 "patBetaParser.cc"
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


#line 283 "/usr/local/lib/bison.cc"
 /* enum token */
     }; /* end of enum declaration */
#endif
public:
 int YY_patBisonBeta_PARSE (YY_patBisonBeta_PARSE_PARAM);
 virtual void YY_patBisonBeta_ERROR(char *msg) YY_patBisonBeta_ERROR_BODY;
#ifdef YY_patBisonBeta_PURE
#ifdef YY_patBisonBeta_LSP_NEEDED
 virtual int  YY_patBisonBeta_LEX (YY_patBisonBeta_STYPE *YY_patBisonBeta_LVAL,YY_patBisonBeta_LTYPE *YY_patBisonBeta_LLOC) YY_patBisonBeta_LEX_BODY;
#else
 virtual int  YY_patBisonBeta_LEX (YY_patBisonBeta_STYPE *YY_patBisonBeta_LVAL) YY_patBisonBeta_LEX_BODY;
#endif
#else
 virtual int YY_patBisonBeta_LEX() YY_patBisonBeta_LEX_BODY;
 YY_patBisonBeta_STYPE YY_patBisonBeta_LVAL;
#ifdef YY_patBisonBeta_LSP_NEEDED
 YY_patBisonBeta_LTYPE YY_patBisonBeta_LLOC;
#endif
 int   YY_patBisonBeta_NERRS;
 int    YY_patBisonBeta_CHAR;
#endif
#if YY_patBisonBeta_DEBUG != 0
 int YY_patBisonBeta_DEBUG_FLAG;   /*  nonzero means print parse trace     */
#endif
public:
 YY_patBisonBeta_CLASS(YY_patBisonBeta_CONSTRUCTOR_PARAM);
public:
 YY_patBisonBeta_MEMBERS 
};
/* other declare folow */
#if YY_patBisonBeta_USE_CONST_TOKEN != 0

/* #line 314 "/usr/local/lib/bison.cc" */
#line 526 "patBetaParser.cc"
const int YY_patBisonBeta_CLASS::pat_gevBeta=258;
const int YY_patBisonBeta_CLASS::pat_gevMu=259;
const int YY_patBisonBeta_CLASS::pat_gevModel=260;
const int YY_patBisonBeta_CLASS::pat_gevScale=261;
const int YY_patBisonBeta_CLASS::pat_gevNests=262;
const int YY_patBisonBeta_CLASS::pat_gevAlt=263;
const int YY_patBisonBeta_CLASS::pat_Attr=264;
const int YY_patBisonBeta_CLASS::pat_UNIF=265;
const int YY_patBisonBeta_CLASS::pat_NORM=266;
const int YY_patBisonBeta_CLASS::patEQUAL=267;
const int YY_patBisonBeta_CLASS::patOB=268;
const int YY_patBisonBeta_CLASS::patCB=269;
const int YY_patBisonBeta_CLASS::patINT=270;
const int YY_patBisonBeta_CLASS::patREAL=271;
const int YY_patBisonBeta_CLASS::patTIME=272;
const int YY_patBisonBeta_CLASS::patNAME=273;
const int YY_patBisonBeta_CLASS::patSTRING=274;
const int YY_patBisonBeta_CLASS::patPAIR=275;


#line 314 "/usr/local/lib/bison.cc"
 /* const YY_patBisonBeta_CLASS::token */
#endif
/*apres const  */
YY_patBisonBeta_CLASS::YY_patBisonBeta_CLASS(YY_patBisonBeta_CONSTRUCTOR_PARAM) YY_patBisonBeta_CONSTRUCTOR_INIT
{
#if YY_patBisonBeta_DEBUG != 0
YY_patBisonBeta_DEBUG_FLAG=0;
#endif
YY_patBisonBeta_CONSTRUCTOR_CODE;
};
#endif

/* #line 325 "/usr/local/lib/bison.cc" */
#line 561 "patBetaParser.cc"


#define	YYFINAL		67
#define	YYFLAG		-32768
#define	YYNTBASE	21

#define YYTRANSLATE(x) ((unsigned)(x) <= 275 ? yytranslate[x] : 49)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20
};

#if YY_patBisonBeta_DEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     4,     6,     8,    11,    15,    17,    19,    23,
    29,    34,    37,    40,    43,    46,    48,    51,    52,    54,
    57,    58,    60,    63,    67,    70,    74,    78,    81,    84,
    86,    89,    92,    95,    97,   100,   103,   105,   107
};

static const short yyrhs[] = {    27,
     0,    39,     0,    22,     0,    23,     0,    22,    23,     0,
     9,    48,    24,     0,    25,     0,    26,     0,    10,    47,
    47,     0,    11,    47,    47,    47,    47,     0,    28,    29,
    30,    31,     0,     3,    32,     0,     4,    36,     0,     5,
    33,     0,     6,    34,     0,    35,     0,    32,    35,     0,
     0,    37,     0,    33,    37,     0,     0,    38,     0,    34,
    38,     0,    46,    47,    48,     0,    47,    48,     0,    46,
    47,    48,     0,    46,    47,    48,     0,    40,    43,     0,
     7,    41,     0,    42,     0,    41,    42,     0,    48,    46,
     0,     8,    44,     0,    45,     0,    44,    45,     0,    48,
    46,     0,    19,     0,    16,     0,    15,     0
};

#endif

#if YY_patBisonBeta_DEBUG != 0
static const short yyrline[] = { 0,
   154,   158,   162,   168,   169,   171,   178,   181,   185,   193,
   203,   206,   209,   212,   215,   218,   219,   221,   222,   223,
   225,   226,   227,   230,   235,   240,   245,   251,   253,   255,
   256,   258,   263,   265,   266,   268,   275,   280,   285
};

static const char * const yytname[] = {   "$","error","$illegal.","pat_gevBeta",
"pat_gevMu","pat_gevModel","pat_gevScale","pat_gevNests","pat_gevAlt","pat_Attr",
"pat_UNIF","pat_NORM","patEQUAL","patOB","patCB","patINT","patREAL","patTIME",
"patNAME","patSTRING","patPAIR","everything","randomdist","rndentry","thedist",
"unifdist","normdist","betasections","betaSection","muSection","modelSection",
"scaleSection","betaValues","modelValues","scaleValues","betaValue","muValue",
"modelValue","scaleValue","nestsections","nestsection","nestNames","nestName",
"altsection","nestAssign","oneAlt","stringParam","floatParam","intParam",""
};
#endif

static const short yyr1[] = {     0,
    21,    21,    21,    22,    22,    23,    24,    24,    25,    26,
    27,    28,    29,    30,    31,    32,    32,    33,    33,    33,
    34,    34,    34,    35,    36,    37,    38,    39,    40,    41,
    41,    42,    43,    44,    44,    45,    46,    47,    48
};

static const short yyr2[] = {     0,
     1,     1,     1,     1,     2,     3,     1,     1,     3,     5,
     4,     2,     2,     2,     2,     1,     2,     0,     1,     2,
     0,     1,     2,     3,     2,     3,     3,     2,     2,     1,
     2,     2,     2,     1,     2,     2,     1,     1,     1
};

static const short yydefact[] = {     0,
     0,     0,     0,     3,     4,     1,     0,     2,     0,    37,
    12,    16,     0,    39,    29,    30,     0,     0,     5,     0,
     0,     0,    28,    17,    38,     0,    31,    32,     0,     0,
     6,     7,     8,    13,     0,    18,     0,    33,    34,     0,
    24,     0,     0,    25,    14,    19,     0,    21,    11,    35,
    36,     9,     0,    20,     0,    15,    22,     0,     0,    26,
    23,     0,    10,    27,     0,     0,     0
};

static const short yydefgoto[] = {    65,
     4,     5,    31,    32,    33,     6,     7,    21,    37,    49,
    11,    45,    56,    12,    34,    46,    57,     8,     9,    15,
    16,    23,    38,    39,    13,    26,    17
};

static const short yypact[] = {    -1,
   -18,    -8,    -8,     0,-32768,-32768,     6,-32768,     3,-32768,
   -18,-32768,     1,-32768,    -8,-32768,   -18,    -6,-32768,     1,
     9,    -8,-32768,-32768,-32768,    -8,-32768,-32768,     1,     1,
-32768,-32768,-32768,-32768,    -8,   -18,    10,    -8,-32768,   -18,
-32768,     1,     1,-32768,   -18,-32768,     1,   -18,-32768,-32768,
-32768,-32768,     1,-32768,    -8,   -18,-32768,     1,     1,-32768,
-32768,    -8,-32768,-32768,    18,    20,-32768
};

static const short yypgoto[] = {-32768,
-32768,    17,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,    11,-32768,   -21,   -29,-32768,-32768,-32768,
    13,-32768,-32768,    -9,    -2,   -17,    -3
};


#define	YYLAST		59


static const short yytable[] = {    18,
    10,     1,    35,    29,    30,     2,    14,     3,     3,    20,
    22,    42,    43,    36,    28,    48,    25,    66,    40,    67,
    19,    24,    41,    54,    52,    53,    61,    27,    50,    55,
     0,    44,     0,    47,    40,    59,     0,    51,     0,     0,
    62,    63,    47,     0,     0,    58,     0,     0,     0,     0,
     0,    60,     0,    58,     0,     0,     0,     0,    64
};

static const short yycheck[] = {     3,
    19,     3,    20,    10,    11,     7,    15,     9,     9,     4,
     8,    29,    30,     5,    17,     6,    16,     0,    22,     0,
     4,    11,    26,    45,    42,    43,    56,    15,    38,    47,
    -1,    35,    -1,    36,    38,    53,    -1,    40,    -1,    -1,
    58,    59,    45,    -1,    -1,    48,    -1,    -1,    -1,    -1,
    -1,    55,    -1,    56,    -1,    -1,    -1,    -1,    62
};

#line 325 "/usr/local/lib/bison.cc"
 /* fattrs + tables */

/* parser code folow  */


/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: dollar marks section change
   the next  is replaced by the list of actions, each action
   as one case of the switch.  */ 

#if YY_patBisonBeta_USE_GOTO != 0
/* 
 SUPRESSION OF GOTO : on some C++ compiler (sun c++)
  the goto is strictly forbidden if any constructor/destructor
  is used in the whole function (very stupid isn't it ?)
 so goto are to be replaced with a 'while/switch/case construct'
 here are the macro to keep some apparent compatibility
*/
#define YYGOTO(lb) {yy_gotostate=lb;continue;}
#define YYBEGINGOTO  enum yy_labels yy_gotostate=yygotostart; \
                     for(;;) switch(yy_gotostate) { case yygotostart: {
#define YYLABEL(lb) } case lb: {
#define YYENDGOTO } } 
#define YYBEGINDECLARELABEL enum yy_labels {yygotostart
#define YYDECLARELABEL(lb) ,lb
#define YYENDDECLARELABEL  };
#else
/* macro to keep goto */
#define YYGOTO(lb) goto lb
#define YYBEGINGOTO 
#define YYLABEL(lb) lb:
#define YYENDGOTO
#define YYBEGINDECLARELABEL 
#define YYDECLARELABEL(lb)
#define YYENDDECLARELABEL 
#endif
/* LABEL DECLARATION */
YYBEGINDECLARELABEL
  YYDECLARELABEL(yynewstate)
  YYDECLARELABEL(yybackup)
/* YYDECLARELABEL(yyresume) */
  YYDECLARELABEL(yydefault)
  YYDECLARELABEL(yyreduce)
  YYDECLARELABEL(yyerrlab)   /* here on detecting error */
  YYDECLARELABEL(yyerrlab1)   /* here on error raised explicitly by an action */
  YYDECLARELABEL(yyerrdefault)  /* current state does not do anything special for the error token. */
  YYDECLARELABEL(yyerrpop)   /* pop the current state because it cannot handle the error token */
  YYDECLARELABEL(yyerrhandle)  
YYENDDECLARELABEL
/* ALLOCA SIMULATION */
/* __HAVE_NO_ALLOCA */
#ifdef __HAVE_NO_ALLOCA
int __alloca_free_ptr(char *ptr,char *ref)
{if(ptr!=ref) free(ptr);
 return 0;}

#define __ALLOCA_alloca(size) malloc(size)
#define __ALLOCA_free(ptr,ref) __alloca_free_ptr((char *)ptr,(char *)ref)

#ifdef YY_patBisonBeta_LSP_NEEDED
#define __ALLOCA_return(num) \
            return( __ALLOCA_free(yyss,yyssa)+\
		    __ALLOCA_free(yyvs,yyvsa)+\
		    __ALLOCA_free(yyls,yylsa)+\
		   (num))
#else
#define __ALLOCA_return(num) \
            return( __ALLOCA_free(yyss,yyssa)+\
		    __ALLOCA_free(yyvs,yyvsa)+\
		   (num))
#endif
#else
#define __ALLOCA_return(num) return(num)
#define __ALLOCA_alloca(size) alloca(size)
#define __ALLOCA_free(ptr,ref) 
#endif

/* ENDALLOCA SIMULATION */

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (YY_patBisonBeta_CHAR = YYEMPTY)
#define YYEMPTY         -2
#define YYEOF           0
#define YYACCEPT        __ALLOCA_return(0)
#define YYABORT         __ALLOCA_return(1)
#define YYERROR         YYGOTO(yyerrlab1)
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL          YYGOTO(yyerrlab)
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do                                                              \
  if (YY_patBisonBeta_CHAR == YYEMPTY && yylen == 1)                               \
    { YY_patBisonBeta_CHAR = (token), YY_patBisonBeta_LVAL = (value);                 \
      yychar1 = YYTRANSLATE (YY_patBisonBeta_CHAR);                                \
      YYPOPSTACK;                                               \
      YYGOTO(yybackup);                                            \
    }                                                           \
  else                                                          \
    { YY_patBisonBeta_ERROR ("syntax error: cannot back up"); YYERROR; }   \
while (0)

#define YYTERROR        1
#define YYERRCODE       256

#ifndef YY_patBisonBeta_PURE
/* UNPURE */
#define YYLEX           YY_patBisonBeta_LEX()
#ifndef YY_USE_CLASS
/* If nonreentrant, and not class , generate the variables here */
int     YY_patBisonBeta_CHAR;                      /*  the lookahead symbol        */
YY_patBisonBeta_STYPE      YY_patBisonBeta_LVAL;              /*  the semantic value of the */
				/*  lookahead symbol    */
int YY_patBisonBeta_NERRS;                 /*  number of parse errors so far */
#ifdef YY_patBisonBeta_LSP_NEEDED
YY_patBisonBeta_LTYPE YY_patBisonBeta_LLOC;   /*  location data for the lookahead     */
			/*  symbol                              */
#endif
#endif


#else
/* PURE */
#ifdef YY_patBisonBeta_LSP_NEEDED
#define YYLEX           YY_patBisonBeta_LEX(&YY_patBisonBeta_LVAL, &YY_patBisonBeta_LLOC)
#else
#define YYLEX           YY_patBisonBeta_LEX(&YY_patBisonBeta_LVAL)
#endif
#endif
#ifndef YY_USE_CLASS
#if YY_patBisonBeta_DEBUG != 0
int YY_patBisonBeta_DEBUG_FLAG;                    /*  nonzero means print parse trace     */
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif
#endif



/*  YYINITDEPTH indicates the initial size of the parser's stacks       */

#ifndef YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif


#if __GNUC__ > 1                /* GNU C and GNU C++ define this.  */
#define __yy_bcopy(FROM,TO,COUNT)       __builtin_memcpy(TO,FROM,COUNT)
#else                           /* not GNU C or C++ */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */

#ifdef __cplusplus
static void __yy_bcopy (char *from, char *to, int count)
#else
#ifdef __STDC__
static void __yy_bcopy (char *from, char *to, int count)
#else
static void __yy_bcopy (from, to, count)
     char *from;
     char *to;
     int count;
#endif
#endif
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}
#endif

int
#ifdef YY_USE_CLASS
 YY_patBisonBeta_CLASS::
#endif
     YY_patBisonBeta_PARSE(YY_patBisonBeta_PARSE_PARAM)
#ifndef __STDC__
#ifndef __cplusplus
#ifndef YY_USE_CLASS
/* parameter definition without protypes */
YY_patBisonBeta_PARSE_PARAM_DEF
#endif
#endif
#endif
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YY_patBisonBeta_STYPE *yyvsp;
  int yyerrstatus;      /*  number of tokens to shift before error messages enabled */
  int yychar1=0;          /*  lookahead token as an internal (translated) token number */

  short yyssa[YYINITDEPTH];     /*  the state stack                     */
  YY_patBisonBeta_STYPE yyvsa[YYINITDEPTH];        /*  the semantic value stack            */

  short *yyss = yyssa;          /*  refer to the stacks thru separate pointers */
  YY_patBisonBeta_STYPE *yyvs = yyvsa;     /*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YY_patBisonBeta_LSP_NEEDED
  YY_patBisonBeta_LTYPE yylsa[YYINITDEPTH];        /*  the location stack                  */
  YY_patBisonBeta_LTYPE *yyls = yylsa;
  YY_patBisonBeta_LTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YY_patBisonBeta_PURE
  int YY_patBisonBeta_CHAR;
  YY_patBisonBeta_STYPE YY_patBisonBeta_LVAL;
  int YY_patBisonBeta_NERRS;
#ifdef YY_patBisonBeta_LSP_NEEDED
  YY_patBisonBeta_LTYPE YY_patBisonBeta_LLOC;
#endif
#endif

  YY_patBisonBeta_STYPE yyval;             /*  the variable used to return         */
				/*  semantic values from the action     */
				/*  routines                            */

  int yylen;
/* start loop, in which YYGOTO may be used. */
YYBEGINGOTO

#if YY_patBisonBeta_DEBUG != 0
  if (YY_patBisonBeta_DEBUG_FLAG)
    fprintf(stderr, "Starting parse\n");
#endif
  yystate = 0;
  yyerrstatus = 0;
  YY_patBisonBeta_NERRS = 0;
  YY_patBisonBeta_CHAR = YYEMPTY;          /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YY_patBisonBeta_LSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
YYLABEL(yynewstate)

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YY_patBisonBeta_STYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YY_patBisonBeta_LSP_NEEDED
      YY_patBisonBeta_LTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YY_patBisonBeta_LSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YY_patBisonBeta_LSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  YY_patBisonBeta_ERROR("parser stack overflow");
	  __ALLOCA_return(2);
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) __ALLOCA_alloca (yystacksize * sizeof (*yyssp));
      __yy_bcopy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      __ALLOCA_free(yyss1,yyssa);
      yyvs = (YY_patBisonBeta_STYPE *) __ALLOCA_alloca (yystacksize * sizeof (*yyvsp));
      __yy_bcopy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
      __ALLOCA_free(yyvs1,yyvsa);
#ifdef YY_patBisonBeta_LSP_NEEDED
      yyls = (YY_patBisonBeta_LTYPE *) __ALLOCA_alloca (yystacksize * sizeof (*yylsp));
      __yy_bcopy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
      __ALLOCA_free(yyls1,yylsa);
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YY_patBisonBeta_LSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YY_patBisonBeta_DEBUG != 0
      if (YY_patBisonBeta_DEBUG_FLAG)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YY_patBisonBeta_DEBUG != 0
  if (YY_patBisonBeta_DEBUG_FLAG)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  YYGOTO(yybackup);
YYLABEL(yybackup)

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* YYLABEL(yyresume) */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    YYGOTO(yydefault);

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (YY_patBisonBeta_CHAR == YYEMPTY)
    {
#if YY_patBisonBeta_DEBUG != 0
      if (YY_patBisonBeta_DEBUG_FLAG)
	fprintf(stderr, "Reading a token: ");
#endif
      YY_patBisonBeta_CHAR = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (YY_patBisonBeta_CHAR <= 0)           /* This means end of input. */
    {
      yychar1 = 0;
      YY_patBisonBeta_CHAR = YYEOF;                /* Don't call YYLEX any more */

#if YY_patBisonBeta_DEBUG != 0
      if (YY_patBisonBeta_DEBUG_FLAG)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(YY_patBisonBeta_CHAR);

#if YY_patBisonBeta_DEBUG != 0
      if (YY_patBisonBeta_DEBUG_FLAG)
	{
	  fprintf (stderr, "Next token is %d (%s", YY_patBisonBeta_CHAR, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, YY_patBisonBeta_CHAR, YY_patBisonBeta_LVAL);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    YYGOTO(yydefault);

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	YYGOTO(yyerrlab);
      yyn = -yyn;
      YYGOTO(yyreduce);
    }
  else if (yyn == 0)
    YYGOTO(yyerrlab);

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YY_patBisonBeta_DEBUG != 0
  if (YY_patBisonBeta_DEBUG_FLAG)
    fprintf(stderr, "Shifting token %d (%s), ", YY_patBisonBeta_CHAR, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (YY_patBisonBeta_CHAR != YYEOF)
    YY_patBisonBeta_CHAR = YYEMPTY;

  *++yyvsp = YY_patBisonBeta_LVAL;
#ifdef YY_patBisonBeta_LSP_NEEDED
  *++yylsp = YY_patBisonBeta_LLOC;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  YYGOTO(yynewstate);

/* Do the default action for the current state.  */
YYLABEL(yydefault)

  yyn = yydefact[yystate];
  if (yyn == 0)
    YYGOTO(yyerrlab);

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
YYLABEL(yyreduce)
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YY_patBisonBeta_DEBUG != 0
  if (YY_patBisonBeta_DEBUG_FLAG)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


/* #line 811 "/usr/local/lib/bison.cc" */
#line 1201 "patBetaParser.cc"

  switch (yyn) {

case 1:
#line 154 "patBetaParser.y"
{
               cout << "Finished parsing  <"
	       << scanner.filename() << ">"<< endl ;
;
    break;}
case 2:
#line 158 "patBetaParser.y"
{
               cout << "Finished parsing  <"
	       << scanner.filename() << ">"<< endl ;
;
    break;}
case 3:
#line 162 "patBetaParser.y"
{
               cout << "Finished parsing  <"
	       << scanner.filename() << ">"<< endl ;
             ;
    break;}
case 6:
#line 171 "patBetaParser.y"
{
  assert(distrib != NULL) ;
  unsigned short p = yyvsp[-1].itype-1 ;
  (*distrib)[p] = *yyvsp[0].dtype ;
  DELETE_PTR(yyvsp[0].dtype) ;
;
    break;}
case 7:
#line 178 "patBetaParser.y"
{
  yyval.dtype = yyvsp[0].dtype ;
;
    break;}
case 8:
#line 181 "patBetaParser.y"
{
  yyval.dtype = yyvsp[0].dtype ;
;
    break;}
case 9:
#line 185 "patBetaParser.y"
{
  patGenerateSimData::patDistrib* d = new patGenerateSimData::patDistrib() ;
  d->type = patGenerateSimData::patUniform ;
  d->lowerBound = yyvsp[-1].ftype ;
  d->upperBound = yyvsp[0].ftype ;
  yyval.dtype = d ;
;
    break;}
case 10:
#line 193 "patBetaParser.y"
{
  patGenerateSimData::patDistrib* d = new patGenerateSimData::patDistrib() ;
  d->type = patGenerateSimData::patNormal ;
  d->mean = yyvsp[-3].ftype ;
  d->stdDev = yyvsp[-2].ftype ;
  d->lowerBound = yyvsp[-1].ftype ;
  d->upperBound = yyvsp[0].ftype ;
  yyval.dtype = d ;
;
    break;}
case 11:
#line 203 "patBetaParser.y"
{
;
    break;}
case 12:
#line 206 "patBetaParser.y"
{
;
    break;}
case 13:
#line 209 "patBetaParser.y"
{
;
    break;}
case 14:
#line 212 "patBetaParser.y"
{
;
    break;}
case 15:
#line 215 "patBetaParser.y"
{
;
    break;}
case 24:
#line 230 "patBetaParser.y"
{
  assert(pMaximizedFunction != NULL) ;
  pMaximizedFunction->addBetaValue(yyvsp[-1].ftype,patString(*yyvsp[-2].stype),yyvsp[0].itype) ;
;
    break;}
case 25:
#line 235 "patBetaParser.y"
{
  assert(pMaximizedFunction != NULL) ;
  pMaximizedFunction->addMuValue(yyvsp[-1].ftype,yyvsp[0].itype) ;
;
    break;}
case 26:
#line 240 "patBetaParser.y"
{
  assert(pMaximizedFunction != NULL) ;
  pMaximizedFunction->addModelValue(yyvsp[-1].ftype,patString(*yyvsp[-2].stype),yyvsp[0].itype) ;
;
    break;}
case 27:
#line 245 "patBetaParser.y"
{
  assert(pMaximizedFunction != NULL) ;
  pMaximizedFunction->addScaleValue(yyvsp[-1].ftype,patString(*yyvsp[-2].stype),yyvsp[0].itype) ;
;
    break;}
case 32:
#line 258 "patBetaParser.y"
{
  assert (nestedLogit  != NULL) ;
  nestedLogit->addNestName(yyvsp[-1].itype,patString(yyvsp[0].stype->c_str())) ;
;
    break;}
case 36:
#line 268 "patBetaParser.y"
{
  assert (nestedLogit  != NULL) ;
  nestedLogit->assignAltToNest(yyvsp[-1].itype,patString(yyvsp[0].stype->c_str())) ;
;
    break;}
case 37:
#line 275 "patBetaParser.y"
{
  string* str = new string((scanner.removeDelimeters()));
  yyval.stype = str ;
;
    break;}
case 38:
#line 280 "patBetaParser.y"
{
  yyval.ftype = atof( scanner.value().c_str() );
;
    break;}
case 39:
#line 285 "patBetaParser.y"
{
  yyval.itype = atoi( scanner.value().c_str() );
;
    break;}
}

#line 811 "/usr/local/lib/bison.cc"
   /* the action file gets copied in in place of this dollarsign  */
  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YY_patBisonBeta_LSP_NEEDED
  yylsp -= yylen;
#endif

#if YY_patBisonBeta_DEBUG != 0
  if (YY_patBisonBeta_DEBUG_FLAG)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YY_patBisonBeta_LSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = YY_patBisonBeta_LLOC.first_line;
      yylsp->first_column = YY_patBisonBeta_LLOC.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  YYGOTO(yynewstate);

YYLABEL(yyerrlab)   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++YY_patBisonBeta_NERRS;

#ifdef YY_patBisonBeta_ERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      YY_patBisonBeta_ERROR(msg);
	      free(msg);
	    }
	  else
	    YY_patBisonBeta_ERROR ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YY_patBisonBeta_ERROR_VERBOSE */
	YY_patBisonBeta_ERROR("parse error");
    }

  YYGOTO(yyerrlab1);
YYLABEL(yyerrlab1)   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (YY_patBisonBeta_CHAR == YYEOF)
	YYABORT;

#if YY_patBisonBeta_DEBUG != 0
      if (YY_patBisonBeta_DEBUG_FLAG)
	fprintf(stderr, "Discarding token %d (%s).\n", YY_patBisonBeta_CHAR, yytname[yychar1]);
#endif

      YY_patBisonBeta_CHAR = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;              /* Each real token shifted decrements this */

  YYGOTO(yyerrhandle);

YYLABEL(yyerrdefault)  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) YYGOTO(yydefault);
#endif

YYLABEL(yyerrpop)   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YY_patBisonBeta_LSP_NEEDED
  yylsp--;
#endif

#if YY_patBisonBeta_DEBUG != 0
  if (YY_patBisonBeta_DEBUG_FLAG)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

YYLABEL(yyerrhandle)

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    YYGOTO(yyerrdefault);

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    YYGOTO(yyerrdefault);

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	YYGOTO(yyerrpop);
      yyn = -yyn;
      YYGOTO(yyreduce);
    }
  else if (yyn == 0)
    YYGOTO(yyerrpop);

  if (yyn == YYFINAL)
    YYACCEPT;

#if YY_patBisonBeta_DEBUG != 0
  if (YY_patBisonBeta_DEBUG_FLAG)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = YY_patBisonBeta_LVAL;
#ifdef YY_patBisonBeta_LSP_NEEDED
  *++yylsp = YY_patBisonBeta_LLOC;
#endif

  yystate = yyn;
  YYGOTO(yynewstate);
/* end loop, in which YYGOTO may be used. */
  YYENDGOTO
}

/* END */

/* #line 1010 "/usr/local/lib/bison.cc" */
#line 1559 "patBetaParser.cc"
#line 293 "patBetaParser.y"



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

 

