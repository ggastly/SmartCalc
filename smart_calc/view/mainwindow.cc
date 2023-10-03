#include "mainwindow.h"

#include "ui_mainwindow.h"

s21::MainWindow::MainWindow(s21::CalcController* controller, QWidget* parent)
    : QMainWindow(parent),
      ui_(new Ui::MainWindow),
      calc_controller_(controller) {
  s21::MainWindow::setFixedSize(740, 393);
  ui_->setupUi(this);
  delete ui_->mainToolBar;
  ConnectButtons();
}

s21::MainWindow::~MainWindow() { delete ui_; }

void s21::MainWindow::ConnectButtons() {
  for (auto button : ui_->calcDigits->buttons()) {
    connect(button, &QPushButton::clicked, this,
            &s21::MainWindow::AppendDisplay);
  }

  for (auto button : ui_->calcFunc->buttons()) {
    connect(button, &QPushButton::clicked, this,
            &s21::MainWindow::AppendDisplayFunctions);
  }
  connect(ui_->buttonEqual, &QPushButton::clicked, this,
          &s21::MainWindow::Calculate);
  connect(ui_->buttonClearAll, &QPushButton::clicked, this,
          &s21::MainWindow::ClearAll);
  connect(ui_->buttonPlot, &QPushButton::clicked, this,
          &s21::MainWindow::CreatePlot);
}

void s21::MainWindow::AppendDisplay() {
  QPushButton* button = (QPushButton*)sender();
  QString new_label = ui_->inputDisplay->text() + button->text();
  ui_->inputDisplay->setText(new_label);
}

void s21::MainWindow::AppendDisplayFunctions() {
  QPushButton* button = (QPushButton*)sender();
  ui_->inputDisplay->setText(ui_->inputDisplay->text() + button->text() + "(");
}

void s21::MainWindow::ClearInput() { ui_->inputDisplay->setText(""); }

void s21::MainWindow::ClearResult() {
  ui_->oldInputDisplay->setText("");
  ui_->resultDisplay->display(0);
}

void s21::MainWindow::ClearAll() {
  ClearInput();
  ClearResult();
}

void s21::MainWindow::OnButtonPointClicked() {
  if (!(ui_->inputDisplay->text().endsWith('.')))
    ui_->inputDisplay->setText(ui_->inputDisplay->text() + ".");
}

void s21::MainWindow::Calculate() {
  ui_->statusBar->showMessage(QString::fromStdString(""));
  std::string input(ui_->inputDisplay->text().toStdString());
  std::string stringX = ui_->inputX->text().toStdString();
  double x = 0.0;
  if (stringX.size() != 0) {
    x = std::stod(stringX);
  }
  double result = calc_controller_->CalculateResult(input, x);
  int error = calc_controller_->GetError();
  if (error) {
    ProcessError(error);
  } else if (result != result) {
    ui_->resultDisplay->display(0.0);
    ui_->statusBar->showMessage(QString::fromStdString("ERROR"));
  } else {
    ui_->resultDisplay->display(result);
    ui_->oldInputDisplay->setText(ui_->inputDisplay->text().append(" ="));
  }
}

void s21::MainWindow::CreatePlot() {
  std::string input_string = ui_->inputDisplay->text().toStdString();
  double x_min = ui_->startX->value();
  double x_max = ui_->endX->value();
  double y_min = ui_->startY->value();
  double y_max = ui_->endY->value();
  double error =
      calc_controller_->BuildGraph(input_string, x_min, x_max, y_min, y_max);
  if (error) {
    ProcessError(error);
  } else {
    UpdateGraph(x_min, x_max, y_min, y_max);
  }
}

void s21::MainWindow::UpdateGraph(double x_min, double x_max, double y_min,
                                  double y_max) {
  ui_->graf->clearGraphs();
  ui_->graf->addGraph();
  ui_->graf->graph(0)->setData(calc_controller_->GetX(),
                               calc_controller_->GetY());
  ui_->graf->xAxis->setLabel("X");
  ui_->graf->yAxis->setLabel("Y");
  ui_->graf->xAxis->setRange(x_min, x_max);
  ui_->graf->yAxis->setRange(y_min, y_max);
  ui_->graf->replot();
}

void s21::MainWindow::ProcessError(int error) {
  if (error == s21::INCORRECT_COORDINATE_SYSTEM) {
    ui_->statusBar->showMessage(
        QString::fromStdString("Coordinate system scale is incorrect!"));
  } else if (error == s21::DIV_ON_ZERO) {
    ui_->statusBar->showMessage(
        QString::fromStdString("You can't divide by zero!"));
  } else if (error == s21::NEGATIVE_SQRT) {
    ui_->statusBar->showMessage(
        QString::fromStdString("SQRT from negative is illegal!"));
  } else {
    ui_->statusBar->showMessage(QString::fromStdString("Incorrect input!"));
  }
}
