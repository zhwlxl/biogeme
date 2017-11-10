//-*-c++-*------------------------------------------------------------
//
// File name : bioEnvVarEditor.h
// Author :    Michel Bierlaire
// Date :      Thu Apr  7 10:04:33 2016
//
//--------------------------------------------------------------------

#ifndef bioEnvVarEditor_h
#define bioEnvVarEditor_h

#include <gtkmm/window.h>
#include <gtkmm/textview.h>
#include <gtkmm/button.h>
#include <gtkmm/grid.h>

#include "patEnvPathVariable.h"

class bioEnvVarEditor : public Gtk::Window {

 public:
  bioEnvVarEditor(vector<patEnvPathVariable> v) ;
  ~bioEnvVarEditor() ;

 protected:
  vector<patEnvPathVariable> theVariables ;
  vector<Gtk::Label> theLabels ;
  vector<Gtk::TextView> theTexts ;
  vector<Glib::RefPtr<Gtk::TextBuffer> > theBuffers ;

  Gtk::Grid grid ;
  Gtk::Button loadButton ;
  Gtk::Button saveButton ;
  Gtk::Button cancelButton ;

};

#endif
