//-*-c++-*------------------------------------------------------------
//
// File name : bioWelcomeWindow.cc
// Author :    Michel Bierlaire
// Date :      Sat Mar  4 11:28:23 2017
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "bioWelcomeWindow.h"
#include <gtkmm/main.h>
#include "logo.h"


bioWelcomeWindow::bioWelcomeWindow() : Gtk::Frame(),
  theWelcome("Welcome to Biogeme"),
  theLogo(Gdk::Pixbuf::create_from_inline (262144,my_pixbuf)) {

  theGrid.set_hexpand(TRUE) ;
  theGrid.set_border_width(10) ;
  theGrid.set_column_spacing(5) ;
  theGrid.set_row_spacing(10) ;

  //  set_position(Gtk::WIN_POS_CENTER);
  
  theGrid.attach(theWelcome,0,0,1,1) ;
  theImage.set(theLogo) ;
  theGrid.attach(theImage,0,1,1,1) ;
  add(theGrid) ;
}


bioWelcomeWindow::~bioWelcomeWindow() {

}

