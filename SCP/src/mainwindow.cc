#include <QtGui>

#include "mainwindow.h"
#include "plotwidget.h"

MainWindow::MainWindow()
{
  createActions();
  createMenus();
  createStatusBar();
  createPlot();
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About Application"),
            tr("Data collection and visualization utility gui for "
               "robotic bicycle experiments."));
}

void MainWindow::connectXBee()
{

}

void MainWindow::createActions()
{
    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    connectXBeeAct = new QAction(tr("Connect to XBee"), this);
    connectXBeeAct->setStatusTip(tr("Connect to an XBee on a serial port"));
    connect(connectXBeeAct, SIGNAL(triggered()), this, SLOT(connectXBee()));

}

void MainWindow::createMenus()
{
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);

    commMenu = menuBar()->addMenu(tr("Communication"));
    commMenu->addAction(connectXBeeAct);
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
