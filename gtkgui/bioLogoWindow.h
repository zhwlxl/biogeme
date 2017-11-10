//-*-c++-*------------------------------------------------------------
//
// File name : bioLogoWindow.h.h
// Author :    Michel Bierlaire
// Date :      Sat Mar  4 11:43:47 2017
//
//--------------------------------------------------------------------

#ifndef bioLogoWindow.h_h
#define bioLogoWindow.h_h

#include <gtkmm/image.h>

class bioLogoWindow.h : public Gtk::Image {
 public: 
  bioLogoWindow() ;
  ~bioLogoWindow() ;
protected:
  Glib::RefPtr< Gdk::Pixbuf >  theLogo ;
  
  
  
} ;

#endif
