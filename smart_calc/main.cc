#include <QApplication>

#include "view/mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  s21::CalcModel calc_model;
  s21::PlotModel plot_model(&calc_model);
  s21::CalcController controller(&calc_model, &plot_model);
  s21::MainWindow view(&controller);
  view.show();
  return a.exec();
}
