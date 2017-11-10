//-*-c++-*------------------------------------------------------------
//
// File name : bioLogWindow.cc
// Author :    Michel Bierlaire
// Date :      Fri Apr  8 16:32:45 2016
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "bioEnvVarEditor.h"

bioEnvVarEditor::bioEnvVarEditor(vector<patEnvPathVariable> v) {

  set_title("Environment variable editor") ;

  loadButton.set_label("Load") ;
  saveButton.set_label("Save") ;
  cancelButton.set_label("Cancel") ;

  grid.set_hexpand(TRUE) ;
  grid.set_border_width(10) ;
  add(grid) ;
  grid.set_column_spacing(5) ;
  grid.set_row_spacing(10) ;
  for (patULong i = 0 ; 
       i < v.size() ;
       ++i) {
    Gtk::Label aLabel ;
    aLabel.set_text(theVariables[i].getName()) ;
    grid.attach(aLabel,0,i,1,1) ;
    theLabels.push_back(aLabel) ;
    Glib::RefPtr<Gtk::TextBuffer> aBuffer = Gtk::TextBuffer::create() ;
    aBuffer->set_text(theVariables[i].getValue()) ;
    theBuffers.push_back(aBuffer) ;
        Gtk::TextView aText ;
    aText.set_buffer(aBuffer) ;
    grid.attach(aText,1,i,1,1) ;
    theTexts.push_back(aText) ;
  }
}
bioEnvVarEditor::~bioEnvVarEditor() {

}
