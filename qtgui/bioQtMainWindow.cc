//-*-c++-*------------------------------------------------------------
//
// File name : bioArithAbs.cc
// Author :    Michel Bierlaire 
// Date :      Sun Feb  7 11:37:17 2016
//
// Implementation inspired by the book "C++ GUI Programming with Qt 4"
// by Jasmin Blanchette and Mark Summerfield
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <QtGui>
#include <QLabel>

#include "bioQtMainWindow.h"

bioQtMainWindow::bioQtMainWindow():
  maxRecentFiles(5) {
  recentModelFileActions.resize(maxRecentFiles,NULL) ;
  recentDataFileActions.resize(maxRecentFiles,NULL) ;
  createActions() ;
  //  createMenus() ;
  createStatusBar() ;
  
  //  readSettings() ;
  //setCurrentModelFile("") ;
  //setCurrentDataFile("") ;

}

bioQtMainWindow::~bioQtMainWindow() {

}

void bioQtMainWindow::createActions() {
  openDataAction = new QAction(tr("&Data"), this);
  openDataAction->setShortcut(tr("Ctrl+D"));
  openDataAction->setStatusTip(tr("Open the data file"));
  connect(openDataAction, SIGNAL(triggered()), this, SLOT(openData()));

  openModelAction = new QAction(tr("&Model"), this);
  openModelAction->setShortcut(tr("Ctrl+M"));
  openModelAction->setStatusTip(tr("Open the model file"));
  connect(openModelAction, SIGNAL(triggered()), this, SLOT(openModel()));

  for (short i = 0; i < maxRecentFiles; ++i) {
    QAction* ptr = new QAction(this); 
    recentModelFileActions[i] = ptr ;
    recentModelFileActions[i]->setVisible(false);
    connect(recentModelFileActions[i], SIGNAL(triggered()),
	    this, SLOT(openRecentModelFile()));

    recentDataFileActions[i] = new QAction(this);
    recentDataFileActions[i]->setVisible(false);
    connect(recentDataFileActions[i], SIGNAL(triggered()),
	    this, SLOT(openRecentDataFile()));

  }

  aboutQtAction = new QAction(tr("About &Qt"), this);
  aboutQtAction->setStatusTip(tr("Show the Qt library’s About box"));
}

void bioQtMainWindow::createMenus() {
  menuBar()->setNativeMenuBar(false);
  modelMenu = menuBar()->addMenu(tr("&Model")) ;
  modelMenu->addAction(openModelAction) ;
  modelSeparatorAction = modelMenu->addSeparator();
  for (short i = 0; i < maxRecentFiles; ++i) {
    modelMenu->addAction(recentModelFileActions[i]);
  }

  dataMenu = menuBar()->addMenu(tr("&Data")) ;
  dataMenu->addAction(openDataAction) ;
  dataSeparatorAction = dataMenu->addSeparator();
  for (short i = 0; i < maxRecentFiles; ++i) {
    dataMenu->addAction(recentModelFileActions[i]);
  }

}

void bioQtMainWindow::openData() {
  QString fileName = QFileDialog::getOpenFileName(this,
						  tr("Open data file"), ".",
						  tr("Data files (*.dat)"));
  if (!fileName.isEmpty()) {
    //    loadFile(fileName);
    QMessageBox::information(this, "Data file", fileName, QMessageBox::Ok);
  }
}

void bioQtMainWindow::openModel() {
  QString fileName = QFileDialog::getOpenFileName(this,
						  tr("Open model file"), ".",
						  tr("Model files (*.mod)"));
  if (!fileName.isEmpty()) {
    //    loadFile(fileName);
    QMessageBox::information(this, "Model file", fileName, QMessageBox::Ok);
  }
}

void bioQtMainWindow::updateRecentModelFileActions() {
  QMutableStringListIterator i(recentModelFiles);
  while (i.hasNext()) {
    if (!QFile::exists(i.next()))
      i.remove();
  }
  for (short j = 0; j < maxRecentFiles; ++j) {
    if (j < recentModelFiles.count()) {
      QString text = tr("&%1 %2")
	.arg(j + 1)
	.arg(strippedName(recentModelFiles[j]));
      recentModelFileActions[j]->setText(text);
      recentModelFileActions[j]->setData(recentModelFiles[j]);
      recentModelFileActions[j]->setVisible(true);
    } else {
      recentModelFileActions[j]->setVisible(false);
    }
  }
  modelSeparatorAction->setVisible(!recentModelFiles.isEmpty());
}

void bioQtMainWindow::updateRecentDataFileActions() {
  QMutableStringListIterator i(recentDataFiles);
  while (i.hasNext()) {
    if (!QFile::exists(i.next()))
      i.remove();
  }
  for (short j = 0; j < maxRecentFiles; ++j) {
    if (j < recentDataFiles.count()) {
      QString text = tr("&%1 %2")
	.arg(j + 1)
	.arg(strippedName(recentDataFiles[j]));
      recentDataFileActions[j]->setText(text);
      recentDataFileActions[j]->setData(recentDataFiles[j]);
      recentDataFileActions[j]->setVisible(true);
    } else {
      recentDataFileActions[j]->setVisible(false);
    }
  }
  dataSeparatorAction->setVisible(!recentDataFiles.isEmpty());
}


void bioQtMainWindow::openRecentModelFile()
{
  QAction *action = qobject_cast<QAction *>(sender());
  if (action) {
    QString fileName = action->data().toString();
    //    loadFile(fileName);
    QMessageBox::information(this, "Model file", fileName, QMessageBox::Ok);
    modelFileLabel->setText(fileName) ;
  }
}

void bioQtMainWindow::openRecentDataFile()
{
  QAction *action = qobject_cast<QAction *>(sender());
  if (action) {
    QString fileName = action->data().toString();
    //    loadFile(fileName);
    QMessageBox::information(this, "Data file", fileName, QMessageBox::Ok);
    dataFileLabel->setText(fileName) ;
  }
}

QString bioQtMainWindow::strippedName(const QString &fullFileName) {
  return QFileInfo(fullFileName).fileName();
}


void bioQtMainWindow::createStatusBar() {

  modelFileLabel = new QLabel("no model loaded") ;
  dataFileLabel = new QLabel("no data loaded") ;
  statusBar()->addWidget(modelFileLabel,1) ;
  statusBar()->addWidget(dataFileLabel,1) ;
}


void bioQtMainWindow::about() {
  QMessageBox::about(this, tr("About Biogeme"),
		     tr("<h2>Biogeme x.xx</h2>"
			"<p>Author: Michel Bierlaire"));
}
