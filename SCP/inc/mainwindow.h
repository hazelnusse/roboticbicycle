#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

 public:
  MainWindow();

private slots:
  void about();

private:
  void createActions();
  void createMenus();
  void createStatusBar();

  QMenu *helpMenu;
  QAction *aboutAct;
};
#endif
