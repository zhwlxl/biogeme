//-*-c++-*------------------------------------------------------------
//
// File name : bioQtMainWindow.h
// Author :    Michel Bierlaire
// Date :      Sun Feb  7 11:36:37 2016
//
//--------------------------------------------------------------------

#ifndef bioQtMainWindow_h
#define bioQtMainWindow_h

#include <QMainWindow>
#include <vector>

class QLabel ;
class bioQtMainWindow: public QMainWindow {
  Q_OBJECT
  
 public:
  bioQtMainWindow() ;
  ~bioQtMainWindow() ;
  // protected:
  //void closeEvent(QCloseEvent *event) ;

private slots:
  void openData();
  void openModel();
  void about();
  void openRecentModelFile() ;
  void openRecentDataFile() ;

 private:
  void createActions() ;
  void createMenus();
  void createStatusBar();
  //  void readSettings();
  //  void writeSettings();
  //  bool loadModelFile(const QString &fileName);
  //  bool loadDataFile(const QString &fileName);
  //  void setCurrentModelFile(const QString &fileName);
  //  void setCurrentDataFile(const QString &fileName);
  void updateRecentModelFileActions();
  void updateRecentDataFileActions();
  QString strippedName(const QString &fullFileName);
  QStringList recentModelFiles;
  QStringList recentDataFiles;
  QString curFile;
  QAction *openModelAction ;
  QAction *openDataAction ;
  QAction *modelSeparatorAction ;
  QAction *dataSeparatorAction ;
  short maxRecentFiles ;
  std::vector<QAction*> recentModelFileActions ;
  std::vector<QAction*> recentDataFileActions ;
  QAction* aboutQtAction ;
  QMenu *modelMenu;
  QMenu *dataMenu;

  QLabel *modelFileLabel ;
  QLabel *dataFileLabel ;

};
#endif
