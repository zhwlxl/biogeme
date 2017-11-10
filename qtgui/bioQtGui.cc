#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <QApplication>
#include "bioQtMainWindow.h"

int main(int argv, char **args)
{
  QApplication app(argv, args);

  bioQtMainWindow  mainWindow ;
  mainWindow.show() ;
  return app.exec();
}
