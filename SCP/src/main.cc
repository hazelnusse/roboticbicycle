#include <QApplication>

#include "mainwindow.h"

int main(int argc, char **argv)
{
  QApplication app(argc, argv);
  app.setOrganizationName("University of California Davis");
  app.setApplicationName("Robotic Bicycle Data Acquisition Tool");
  MainWindow mainWin;
  mainWin.setGeometry(0, 0, 1024, 768);
  mainWin.show();
  return app.exec();
}
