#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>
#include <vtkSmartPointer.h>

class QVTKWidget;
class vtkContextView;
class vtkContextScene;
class vtkTable;
class vtkShortArray;
class vtkUnsignedShortArray;
class vtkChartXY;
class vtkPlot;
QT_BEGIN_NAMESPACE
class QSpinBox;
class QGroupBox;
QT_END_NAMESPACE

class PlotWidget : public QWidget
{
 Q_OBJECT

 public:
  PlotWidget(QWidget *par = 0);

 protected:
  // void paintEvent(QPaintEvent *event);
 
 private slots:
  void DataPointsChanged();

 private:
  void createSignalGroup();
  void createPlotWidget();

  QSpinBox *dataPoints;
  QVTKWidget *qvtkWidget;
  QGroupBox *signalsGroup;
  vtkSmartPointer<vtkContextView> view;
  vtkSmartPointer<vtkTable> table;
  vtkSmartPointer<vtkShortArray> gt, gx, gy, gz, ax, ay, az, mx, my, mz;
  vtkSmartPointer<vtkUnsignedShortArray> st, rw, fw, t;
  vtkSmartPointer<vtkChartXY> chart;
  vtkPlot *line;
};
#endif
