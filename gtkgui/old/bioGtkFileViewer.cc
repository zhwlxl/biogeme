//-*-c++-*------------------------------------------------------------
//
// File name : bioGtkFileViewer.cc
// Author :    Michel Bierlaire
// Date :      Wed Apr 27 08:22:28 2016
//
//--------------------------------------------------------------------

#include <giomm.h>
#include <gtkmm/messagedialog.h>
#include "patDisplay.h"
#include "patVersion.h"
//
// WARNING. the content type "text/plain" is only valid in unix. In windows, ot should be "*.txt". 

// see https://developer.gnome.org/gio/stable/gio-GContentType.html

#include <gtkmm/label.h>
#include "bioGtkFileViewer.h"
bioGtkFileViewer::bioGtkFileViewer() : introduction("Click the corresponding button to copy the URI, and paste it in your favorite browser")  {
  initWindow() ;

}

bioGtkFileViewer::bioGtkFileViewer(std::vector<std::pair<Glib::ustring,Glib::ustring > > lof) : introduction("Click the corresponding button to copy the URI, and paste it in your favorite browser") {
  for (std::size_t i = 0 ; i < lof.size() ; ++i) {
    addFile(lof[i]) ;
  }

  initWindow() ;
}

void bioGtkFileViewer::initWindow() {
  stringstream str ; 
  str << patVersion::the()->getVersionInfo() << ": file viewer" ;
  set_title(str.str()) ;
  set_default_size(600,400) ;
  add(grid) ;
  grid.set_hexpand(TRUE) ;
  grid.set_border_width(10) ;
  grid.set_column_spacing(5) ;
  grid.set_row_spacing(10) ;

  grid.attach(introduction,0,0,3,1) ;
  for (std::size_t i = 0 ; i < listOfButtons.size() ; ++i) {
    grid.attach(listOfLabels[i],0,i+1,1,1) ;
    grid.attach(listOfFileNames[i],1,i+1,1,1) ;
    grid.attach(listOfButtons[i],2,i,1+1,1) ;
  }



}

bioGtkFileViewer::~bioGtkFileViewer() {

}


void bioGtkFileViewer::addFile(std::pair<Glib::ustring,Glib::ustring > f) {
  std::size_t newposition = listOfButtons.size() ;
  listOfFiles.push_back(f.first) ;
  listOfFileNames.push_back(Gtk::Label(f.first)) ;
  listOfLabels.push_back(Gtk::Label(f.second)) ;
  
  listOfButtons.push_back(Gtk::Button("Copy URI")) ;

  listOfButtons[newposition].signal_clicked().connect(sigc::bind<std::size_t>(sigc::mem_fun(*this,&bioGtkFileViewer::openFile),newposition)) ;
  listOfButtons[newposition].set_tooltip_text("Copy the Uniform Resource Identifier (URI) of the file to be pasted in the browser") ;
  grid.attach(listOfLabels[newposition],0,newposition+1,1,1) ;
  grid.attach(listOfFileNames[newposition],1,newposition+1,1,1) ;
  grid.attach(listOfButtons[newposition],2,newposition+1,1,1) ;
}

void bioGtkFileViewer::openFile(std::size_t findex) {
  Glib::RefPtr<Gio::File> theFile = 
    Gio::File::create_for_path(listOfFiles[findex]);

  Glib::RefPtr<Gio::FileInfo> theFileInfo = theFile->query_info("*") ;

  Glib::ustring uri = theFile->get_uri() ;

  auto refClipboard = Gtk::Clipboard::get() ;
  refClipboard->set_text(uri);
  return ;


  GError *g = NULL;
  GError anerr = { 0, 0, NULL } ; 
  gtk_show_uri(NULL,uri.c_str(),GDK_CURRENT_TIME,&g) ;
  
  if (g != NULL) {
    stringstream str1 ;
    stringstream str2 ;
    str1 << "Error: unable to open  " 
	 << uri ;
    Gtk::MessageDialog dialog(*this,Glib::ustring(str1.str())) ;
    anerr = *g;
    if( !anerr.message || !*anerr.message ) {
      str2 << "an unknown error has occurred" ;
      dialog.set_secondary_text(str2.str());
      dialog.run();
    }
  }
  if (anerr.message) {
    stringstream str1 ;
    stringstream str2 ;
    str1 << "Error: unable to open  " 
	 << uri ;
    Gtk::MessageDialog dialog(*this,Glib::ustring(str1.str())) ;
    str2 << " Reason: " 
	 << anerr.message ;
    dialog.set_secondary_text(str2.str());
    dialog.run();
  }    
}
