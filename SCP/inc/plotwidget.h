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

class PlotWidget : public QWidget
{
 Q_OBJECT

 public:
  PlotWidget(QWidget *parent = 0);

 protected:
  // void paintEvent(QPaintEvent *event);

 private:
  vtkSmartPointer<vtkContextView> view;
  vtkSmartPointer<vtkTable> table;
  vtkSmartPointer<vtkShortArray> gt, gx, gy, gz, ax, ay, az, mx, my, mz;
  vtkSmartPointer<vtkUnsignedShortArray> st, rw, fw, t;
  vtkSmartPointer<vtkChartXY> chart;
  vtkPlot *line;
};
#endif
