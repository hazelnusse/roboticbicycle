#include <QtGui>

#include "mainwindow.h"

MainWindow::MainWindow()
{
    createActions();
    createMenus();
    createStatusBar();
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About Application"),
            tr("Data collection and visualization utility gui for "
               "robotic bicycle experiments."));
}

void MainWindow::createActions()
{
    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::createStatusBar()
{
  statusBar()->showMessage(tr("Ready"));
}
