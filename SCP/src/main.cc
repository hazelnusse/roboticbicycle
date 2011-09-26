#include <QApplication>

#include "mainwindow.h"

int main(int argc, char **argv)
{
  QApplication app(argc, argv);
  app.setOrganizationName("Universityf of California Davis");
  app.setApplicationName("Robotic Bicycle Data Acquisition Tool");
  MainWindow mainWin;
  mainWin.show();
  return app.exec();
}
