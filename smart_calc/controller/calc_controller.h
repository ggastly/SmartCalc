#ifndef CPP3_SMARTCALC_V2_SRC_CALC_CONTROLLER_H_
#define CPP3_SMARTCALC_V2_SRC_CALC_CONTROLLER_H_

#include "../models/calculator_model.h"
#include "../models/plot_model.h"

namespace s21 {

class CalcController {
 public:
  CalcController(){};

  CalcController(CalcModel *calc_model, PlotModel *plot_model)
      : calc_model_(calc_model), plot_model_(plot_model) {}

  double CalculateResult(std::string &input_string, double input_x) {
    return calc_model_->CalculateResult(input_string, input_x);
  }

  int BuildGraph(std::string &input_string, double start_x, double end_x,
                 double start_y, double end_y) {
    return plot_model_->BuildPlot(input_string, start_x, end_x, start_y, end_y);
  }

  int GetError() { return calc_model_->GetError(); }

  QVector<double> GetX() { return plot_model_->GetX(); }

  QVector<double> GetY() { return plot_model_->GetY(); }

 private:
  CalcModel *calc_model_;
  PlotModel *plot_model_;
};

}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_SRC_CALC_CONTROLLER_H_
