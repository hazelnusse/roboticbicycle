#ifndef XBEEDIALOG_H
#define XBEEDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QDataStream;
QT_END_NAMESPACE

class XBeeDialog : public QDialog
{
  Q_OBJECT

 public:
  XBeeDialog(QDataStream *stream, QWidget *par = 0);

 private slots:
  void OpenClicked();
  void ResetClicked();
  void CloseClicked();

 private:
  void setupSerialPort() const;
  void resetXBee() const;
  QPushButton *OpenButton, *CloseButton, *ResetButton;
  QLabel *fileNameLabel;
  QDataStream * const xbStream;
};
#endif
