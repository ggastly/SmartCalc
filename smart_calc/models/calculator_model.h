#ifndef CPP3_SMARTCALC_V2_SRC_CALCULATOR_MODEL_H_
#define CPP3_SMARTCALC_V2_SRC_CALCULATOR_MODEL_H_

#include <cmath>

#include "converter_model.h"

namespace s21 {

class CalcModel {
 public:
  using Stack =
      std::stack<ConverterModel::Lexem, std::list<ConverterModel::Lexem>>;

  CalcModel() {}

  double CalculateResult(std::string &input_infix_string, double x) {
    double result = 0;
    ValidatorModel validator(input_infix_string);
    ConverterModel converter(input_infix_string);
    error_ = validator.Validate();
    if (!error_) {
      Stack stack_with_rpn = converter.FromInfixStringToPostfixStack(x);
      result = CalculateRPN(stack_with_rpn);
    };
    return result;
  }

  int GetError() { return error_; }

 private:
  double CalculateRPN(Stack &stack_rpn) {
    double result = 0.0;
    if (!stack_rpn.empty()) {
      ConverterModel::Lexem top_lexem = stack_rpn.top();
      stack_rpn.pop();
      if (top_lexem.type == NUM) {
        result = top_lexem.value;
      } else if (top_lexem.type == FUNC) {
        double param = CalculateRPN(stack_rpn);
        if (param < 0 && top_lexem.func == SQRT) {
          error_ = s21::NEGATIVE_SQRT;
        } else {
          result = CalculateUnaryOperators(top_lexem.func, param);
        }
      } else {
        double b = CalculateRPN(stack_rpn);
        if (b == 0 && top_lexem.type == DIV) {
          error_ = s21::DIV_ON_ZERO;
        } else {
          double a = CalculateRPN(stack_rpn);
          result = CalculateBinaryOperators(a, b, top_lexem.type);
        }
      }
    }
    return result;
  }

  double CalculateUnaryOperators(int func, double b) {
    double result = 0.0;
    if (func == COS) {
      result = cos(b);
    } else if (func == SIN) {
      result = sin(b);
    } else if (func == TAN) {
      result = tan(b);
    } else if (func == ACOS) {
      result = acos(b);
    } else if (func == ASIN) {
      result = asin(b);
    } else if (func == ATAN) {
      result = atan(b);
    } else if (func == SQRT) {
      result = sqrt(b);
    } else if (func == LN) {
      result = log(b);
    } else if (func == LOG) {
      result = log10(b);
    }
    return result;
  }

  double CalculateBinaryOperators(double a, double b, int type) {
    double result = 0.0;
    if (type == EXP) {
      result = pow(a, b);
    } else if (type == MOD) {
      result = fmod(a, b);
    } else if (type == MULT) {
      result = a * b;
    } else if (type == DIV) {
      result = a / b;
    } else if (type == ADD || type == UN_ADD) {
      result = a + b;
    } else if (type == SUB || type == UN_SUB) {
      result = a - b;
    }
    return result;
  }

 private:
  int error_ = 0;
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_SRC_CALCULATOR_MODEL_H_
