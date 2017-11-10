//-*-c++-*------------------------------------------------------------
//
// File name : bioWelcomeWindow.h
// Author :    Michel Bierlaire
// Date :      Sat Mar  4 11:27:22 2017
//
//--------------------------------------------------------------------

#ifndef bioWelcomeWindow_h
#define bioWelcomeWindow_h

#include <gtkmm/frame.h>
#include <gtkmm/label.h>
#include <gtkmm/image.h>
#include <gtkmm/grid.h>


class bioWelcomeWindow : public Gtk::Frame {
 public: 
  bioWelcomeWindow() ;
  ~bioWelcomeWindow() ;
protected:
  
  Gtk::Grid theGrid ;
  Gtk::Label theWelcome ;
  Gtk::Image theImage ;
  Glib::RefPtr< Gdk::Pixbuf >  theLogo ;
  
} ;

#endif
