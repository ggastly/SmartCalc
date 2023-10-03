#ifndef CPP3_SMARTCALC_V2_SRC_PLOT_MODEL_H_
#define CPP3_SMARTCALC_V2_SRC_PLOT_MODEL_H_

#include "calculator_model.h"
#include "qcustomplot.h"

namespace s21 {

class PlotModel {
 public:
  PlotModel() {}

  PlotModel(CalcModel *calc_model) : calc_model_(calc_model) {}

  int BuildPlot(std::string &input_string, double x_min, double x_max,
                double y_min, double y_max) {
    if (x_min >= x_max || y_min >= y_max) {
      return INCORRECT_COORDINATE_SYSTEM;
    }
    GeneratePlotData(input_string, x_min, x_max, y_min, y_max);
    return OK;
  }

  QVector<double> GetX() { return x_; }

  QVector<double> GetY() { return y_; }

 private:
  void GeneratePlotData(std::string &input_string, double x_min, double x_max,
                        double y_min, double y_max) {
    double h = (x_max - x_min) / 1500;
    x_.clear();
    y_.clear();
    for (double X = x_min; X <= x_max; X += h) {
      double Y = calc_model_->CalculateResult(input_string, X);
      int error_ = calc_model_->GetError();
      if ((!error_ || error_ == NEGATIVE_SQRT) && Y <= y_max && Y >= y_min) {
        x_.push_back(X);
        y_.push_back(Y);
      }
    }
  }

 private:
  QVector<double> x_, y_;
  CalcModel *calc_model_;
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_SRC_PLOT_MODEL_H_
