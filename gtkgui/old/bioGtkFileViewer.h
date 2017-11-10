//-*-c++-*------------------------------------------------------------
//
// File name : bioGtkFileViewer.h
// Author :    Michel Bierlaire
// Date :      Mon Apr 25 17:18:08 2016
//
//--------------------------------------------------------------------

#ifndef bioGtkFileViewer_h
#define bioGtkFileViewer_h

#include <vector>
#include <gtkmm/window.h>
#include <gtkmm/grid.h>
#include <gtkmm/button.h>

class bioGtkFileViewer : public Gtk::Window {
public:
  bioGtkFileViewer() ;
  bioGtkFileViewer(std::vector<std::pair<Glib::ustring,Glib::ustring > > lof) ;
  ~bioGtkFileViewer() ;
  void addFile(std::pair<Glib::ustring,Glib::ustring > f) ;
protected:
  Gtk::Grid grid ;
  Gtk::Label introduction ;
  std::vector<Gtk::Label> listOfLabels ;
  std::vector<Gtk::Label> listOfFileNames ;
  std::vector<Gtk::Button> listOfButtons ;
  std::vector<Glib::ustring> listOfFiles ;

  void openFile(std::size_t findex) ;
private:
  void initWindow() ;
};
#endif
