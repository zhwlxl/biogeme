//-*-c++-*------------------------------------------------------------
//
// File name : bioGtkAssistant.h
// Author :    Michel Bierlaire
// Date :      Sat Mar  4 18:09:01 2017
//
//--------------------------------------------------------------------

#ifndef bioGtkAssistant_h
#define bioGtkAssistant_h

#include <gtkmm/assistant.h>
#include "bioWelcomeWindow.h" 

class bioGtkAssistant: public Gtk::Assistant {
 public:
  bioGtkAssistant() ;
  virtual ~bioGtkAssistant() ;

  void appendPages() ;
 protected:

  bioWelcomeWindow theWelcomeWindow ;

};
#endif
