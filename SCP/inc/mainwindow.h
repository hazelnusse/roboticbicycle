#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "sample.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class XBeeDialog;
class QFile;
class QDataStream;
class QTimer;
template<class Sample> class QQueue;
QT_END_NAMESPACE 

class PlotWidget;
class DataReader;

class MainWindow : public QMainWindow
{
  Q_OBJECT

 public:
  MainWindow();
  ~MainWindow();
  QTimer *timer() const;
  DataReader *dataReader() const;

private slots:
  void about();
  void connectXBee();

private:
  void createActions();
  void createMenus();
  void createStatusBar();

  void createPlot();

  void createXBeeData();
  void createXBeeDialog();
  void createTimer();
  void createDataReader();

  // XBee file, stream, configuration dialog
  QFile *xbFile;
  QDataStream *xbStream;
  XBeeDialog *xbDialog;

  // Data Reader Worker
  DataReader *reader;

  // Sample Queue
  QQueue<Sample> *SampleQueue;

  // Timer for looking for new data
  QTimer *PlotUpdateTimer;

  // GUI Stuff
  QMenu *helpMenu;
  QMenu *commMenu;
  QAction *aboutAct;
  QAction *connectXBeeAct;
  PlotWidget *dataPlot;
};
#endif
