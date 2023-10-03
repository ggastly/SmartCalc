#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QKeyEvent>
#include <QKeySequence>
#include <QMainWindow>
#include <QString>
#include <QVector>
#include <QWidget>
#include <QtMath>

#include "../controller/calc_controller.h"

namespace Ui {
class MainWindow;
}

namespace s21 {

class MainWindow : public QMainWindow {
  Q_OBJECT

  using QWidget::keyPressEvent;

 public:
  explicit MainWindow(CalcController *controller, QWidget *parent = nullptr);
  ~MainWindow();

 private:
  void ConnectButtons();
  void AppendDisplay();
  void ClearResult();
  void ClearInput();
  void ClearAll();
  void Calculate();
  void OnButtonPointClicked();
  void UpdateGraph(double x_min, double x_max, double y_min, double y_max);
  void AppendDisplayFunctions();
  void CreatePlot();
  void ProcessError(int error);

 private:
  Ui::MainWindow *ui_;
  CalcController *calc_controller_;
};

}  // namespace s21

#endif  // MAINWINDOW_H
