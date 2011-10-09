#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE

class PlotWidget;

class MainWindow : public QMainWindow
{
  Q_OBJECT

 public:
  MainWindow();

private slots:
  void about();
  void connectXBee();

private:
  void createActions();
  void createMenus();
  void createStatusBar();
  void createPlot();

  QMenu *helpMenu;
  QMenu *commMenu;
  QAction *aboutAct;
  QAction *connectXBeeAct;
  PlotWidget *dataPlot;
};
#endif
