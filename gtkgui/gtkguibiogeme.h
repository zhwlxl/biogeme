//-*-c++-*------------------------------------------------------------
//
// File name : gtkguibiogeme.h
// Author :    Michel Bierlaire
// Date :      Fri Mar 25 11:13:18 2016
//
//--------------------------------------------------------------------


#ifndef gtkguibiogeme_h
#define gtkguibiogeme_h

enum bioApplicationVersion {
  bioBison,
  bioSim,
  bioPython
};
 

typedef struct  {
  patString modelFileWithPath ;
  patString dataFileWithPath ;
  patBoolean modelSelected ;
  patBoolean dataSelected ;
  bioApplicationVersion theVersion ;
}  bioRunCallbackData ;

#endif
