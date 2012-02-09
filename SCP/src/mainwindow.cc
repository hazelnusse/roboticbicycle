#include <QtGui>
#include <QFile>
#include <QQueue>
#include <QTimer>

#include "mainwindow.h"
#include "plotwidget.h"
#include "xbeedialog.h"
#include "datareader.h"

MainWindow::MainWindow()
{
  createActions();
  createMenus();
  createStatusBar();

  createPlot();

  createXBeeData();   // Sets up the xbStream and SampleQueue
  createXBeeDialog(); // Setups the dialog
  createTimer();
  createDataReader();
}

MainWindow::~MainWindow()
{
  if (xbFile->isOpen())
    xbFile->close();

  delete xbStream;
  delete xbFile;
  delete SampleQueue;
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About Application"),
            tr("Data collection and visualization utility gui for "
               "robotic bicycle experiments."));
}

void MainWindow::connectXBee()
{
  xbDialog->show();
  xbDialog->raise();
  xbDialog->activateWindow();
}

void MainWindow::createActions()
{
  aboutAct = new QAction(tr("&About"), this);
  aboutAct->setStatusTip(tr("Show the application's About box"));
  connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

  connectXBeeAct = new QAction(tr("Connect to &XBee"), this);
  connectXBeeAct->setStatusTip(tr("Connect to an XBee on a serial port"));
  connect(connectXBeeAct, SIGNAL(triggered()), this, SLOT(connectXBee()));
}

void MainWindow::createMenus()
{
  commMenu = menuBar()->addMenu(tr("&Communication"));
  commMenu->addAction(connectXBeeAct);

  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(aboutAct);
}

void MainWindow::createStatusBar()
{
  statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createPlot()
{
  dataPlot = new PlotWidget(this);
  setCentralWidget(dataPlot);
}

void MainWindow::createTimer()
{
  PlotUpdateTimer = new QTimer(this);
  PlotUpdateTimer->setInterval(1000);
  PlotUpdateTimer->setSingleShot(false);
}

void MainWindow::createDataReader()
{
  reader = new DataReader(this, SampleQueue);
  connect(PlotUpdateTimer, SIGNAL(timeout()), reader, SLOT(read()));
}

void MainWindow::createXBeeData()
{
  xbFile = new QFile;
  xbStream = new QDataStream(xbFile);
  SampleQueue = new QQueue<Sample>;
}

void MainWindow::createXBeeDialog()
{
  xbDialog = new XBeeDialog(xbStream, this);
}

QTimer * MainWindow::timer() const
{
  return PlotUpdateTimer;
}

DataReader * MainWindow::dataReader() const
{
  return reader;
}
