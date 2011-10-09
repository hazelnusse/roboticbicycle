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

PlotWidget::PlotWidget(QWidget *parent)
  : QWidget(parent),
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
  QVTKWidget *qvtkWidget = new QVTKWidget(this);

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

  // Test charting with a few more points...
  int numPoints = 200;
  table->SetNumberOfRows(numPoints);
  for (int i = 0; i < numPoints; ++i) {
    for (int j = 0; j < 13; ++j) {
        table->SetValue(i, j, i*j);
    }
  }
  table->Update();

  view->GetScene()->AddItem(chart);

  line = chart->AddPlot(vtkChart::LINE);
  line->SetInput(table, 12, 1);
  line->SetColor(255, 0, 0, 255);

  QGroupBox *signalsGroup = new QGroupBox(tr("Signal selection"));
  QCheckBox *gyroCheckBox = new QCheckBox(tr("Gyroscope"));
  QCheckBox *accCheckBox = new QCheckBox(tr("Accelerometer"));
  QCheckBox *magCheckBox = new QCheckBox(tr("Magnetometer"));
  QCheckBox *steerCheckBox = new QCheckBox(tr("Steer"));
  QCheckBox *rearWheelCheckBox = new QCheckBox(tr("Rear Wheel"));
  QCheckBox *frontWheelCheckBox = new QCheckBox(tr("Front Wheel"));

  QVBoxLayout *signalsLayout = new QVBoxLayout;
  signalsLayout->addWidget(gyroCheckBox);
  signalsLayout->addWidget(accCheckBox);
  signalsLayout->addWidget(magCheckBox);
  signalsLayout->addWidget(steerCheckBox);
  signalsLayout->addWidget(rearWheelCheckBox);
  signalsLayout->addWidget(frontWheelCheckBox);
  signalsGroup->setLayout(signalsLayout);

  QHBoxLayout *l = new QHBoxLayout(this);
  l->addWidget(qvtkWidget);
  l->addWidget(signalsGroup);

  setLayout(l);
}
