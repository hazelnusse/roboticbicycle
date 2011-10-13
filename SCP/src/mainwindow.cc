#include <QtGui>
#include <QFile>

#include "mainwindow.h"
#include "plotwidget.h"
#include "xbeedialog.h"

MainWindow::MainWindow()
{
  createActions();
  createMenus();
  createStatusBar();

  xbFile = new QFile;
  xbStream = new QDataStream(xbFile);

  createPlot();
  createXBeeDialog();
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

void MainWindow::createXBeeDialog()
{
  xbDialog = new XBeeDialog(xbStream, this);
}
