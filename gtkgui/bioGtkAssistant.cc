//-*-c++-*------------------------------------------------------------
//
// File name : bioGtkAssistant.cc
// Author :    Michel Bierlaire
// Date :      Sat Mar  4 18:11:37 2017
//
//--------------------------------------------------------------------


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <iostream>

#include "bioGtkAssistant.h"

bioGtkAssistant::bioGtkAssistant() {
  
}

bioGtkAssistant::~bioGtkAssistant() {
}

void bioGtkAssistant::appendPages() {
  append_page(theWelcomeWindow) ;

}



