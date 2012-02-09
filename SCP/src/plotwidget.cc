#include <QtGui>

#include "QVTKWidget.h"
#include "vtkContextView.h"
#include "vtkContextScene.h"
#include "vtkTable.h"
#include "vtkShortArray.h"
#include "vtkUnsignedShortArray.h"
#include "vtkChartXY.h"
#include "vtkPlot.h"

#include "plotwidget.h"

PlotWidget::PlotWidget(QWidget *par)
  : QWidget(par),
    view(vtkSmartPointer<vtkContextView>::New()),
    table(vtkSmartPointer<vtkTable>::New()),
    gt(vtkSmartPointer<vtkShortArray>::New()),
    gx(vtkSmartPointer<vtkShortArray>::New()),
    gy(vtkSmartPointer<vtkShortArray>::New()),
    gz(vtkSmartPointer<vtkShortArray>::New()),
    ax(vtkSmartPointer<vtkShortArray>::New()),
    ay(vtkSmartPointer<vtkShortArray>::New()),
    az(vtkSmartPointer<vtkShortArray>::New()),
    mx(vtkSmartPointer<vtkShortArray>::New()),
    my(vtkSmartPointer<vtkShortArray>::New()),
    mz(vtkSmartPointer<vtkShortArray>::New()),
    st(vtkSmartPointer<vtkUnsignedShortArray>::New()),
    rw(vtkSmartPointer<vtkUnsignedShortArray>::New()),
    fw(vtkSmartPointer<vtkUnsignedShortArray>::New()),
    t(vtkSmartPointer<vtkUnsignedShortArray>::New()),
    chart(vtkSmartPointer<vtkChartXY>::New())
{
  createSignalGroup();
  createPlotWidget();

  QVBoxLayout *l = new QVBoxLayout(this);
  l->addWidget(qvtkWidget);
  l->addWidget(signalsGroup);
  setLayout(l);
}

void PlotWidget::createSignalGroup()
{
  signalsGroup = new QGroupBox(tr("Signal selection"));
  QCheckBox *gyroCheckBox = new QCheckBox(tr("Gyroscope"));
  QCheckBox *accCheckBox = new QCheckBox(tr("Accelerometer"));
  QCheckBox *magCheckBox = new QCheckBox(tr("Magnetometer"));
  QCheckBox *steerCheckBox = new QCheckBox(tr("Steer"));
  QCheckBox *rearWheelCheckBox = new QCheckBox(tr("Rear Wheel"));
  QCheckBox *frontWheelCheckBox = new QCheckBox(tr("Front Wheel"));

  QLabel *dataPointsLabel = new QLabel(tr("Number of Samples"));
  dataPoints = new QSpinBox;
  dataPoints->setMaximum(20000);
  dataPoints->setMinimum(600);
  dataPoints->setValue(1000);

  QGridLayout *signalsLayout = new QGridLayout;
  signalsLayout->addWidget(gyroCheckBox, 0, 0);
  signalsLayout->addWidget(accCheckBox, 0, 1);
  signalsLayout->addWidget(magCheckBox, 0, 2);
  signalsLayout->addWidget(dataPointsLabel, 0, 3);
  signalsLayout->addWidget(steerCheckBox, 1, 0);
  signalsLayout->addWidget(rearWheelCheckBox, 1, 1);
  signalsLayout->addWidget(frontWheelCheckBox, 1, 2);
  signalsLayout->addWidget(dataPoints, 1, 3);
  signalsGroup->setLayout(signalsLayout);
}

void PlotWidget::createPlotWidget()
{
  qvtkWidget = new QVTKWidget(this);

  view->SetInteractor(qvtkWidget->GetInteractor());
  qvtkWidget->SetRenderWindow(view->GetRenderWindow());

  gt->SetName("Gyro T"); table->AddColumn(gt);
  gx->SetName("Gyro X"); table->AddColumn(gx);
  gy->SetName("Gyro Y"); table->AddColumn(gy);
  gz->SetName("Gyro Z"); table->AddColumn(gz);
  ax->SetName("Acc X"); table->AddColumn(ax);
  ay->SetName("Acc Y"); table->AddColumn(ay);
  az->SetName("Acc Z"); table->AddColumn(az);
  mx->SetName("Mag X"); table->AddColumn(mx);
  my->SetName("Mag Y"); table->AddColumn(my);
  mz->SetName("Mag Z"); table->AddColumn(mz);
  st->SetName("Steer"); table->AddColumn(st);
  rw->SetName("Rear wheel"); table->AddColumn(rw);
  fw->SetName("Front wheel"); table->AddColumn(fw);
  t->SetName("Time"); table->AddColumn(t);


  int numPoints = dataPoints->value();
  table->SetNumberOfRows(dataPoints->value());
  for (int i = 0; i < numPoints; ++i) {
    int j;
    for (j = 0; j < 13; ++j) {
        table->SetValue(i, j, j);
    }
    table->SetValue(i, j, i);

  }
  table->Update();

  vtkPlot *lines[13];
  for (int j = 0; j < 13; ++j) {
    lines[j] = chart->AddPlot(vtkChart::LINE);
    lines[j]->SetInput(table, 13, j);
    lines[j]->SetColor(255, 0, 0, 255);
  }
  view->GetScene()->AddItem(chart);
  chart->SetShowLegend(true);
}

void PlotWidget::DataPointsChanged()
{
  table->SetNumberOfRows(dataPoints->value());
  table->Update();
}
