#ifndef CPP3_SMARTCALC_V2_SRC_MODEL_CONVERTER_MODEL_H_
#define CPP3_SMARTCALC_V2_SRC_MODEL_CONVERTER_MODEL_H_

#include <cstring>
#include <list>
#include <regex>
#include <sstream>
#include <stack>

#include "validator_model.h"

namespace s21 {

enum SpecialLexems {
  NUM,
  LEFT_BRACKET,
  RIGHT_BRACKET,
  EXP,
  MOD,
  MULT,
  DIV,
  ADD,
  SUB,
  FUNC,
  X,
  UN_ADD,
  UN_SUB
};
enum Funcs { NONE, COS, SIN, TAN, ACOS, ASIN, ATAN, SQRT, LN, LOG };
enum Priorities { ZERO, FIRST, SECOND, THIRD, FOURTH };

class ConverterModel {
 public:
  typedef struct Lexem {
    double value;
    int type;
    int func;
    int priority;
  } Lexem;

  using Stack = std::stack<Lexem, std::list<Lexem>>;
  using StringIterator = std::string::const_iterator;

  ConverterModel(std::string &input_infix_string) {
    it_symbol_ = input_infix_string.begin();
    it_symbol_end_ = input_infix_string.end();
    it_symbol_begin_ = it_symbol_;
  };

  Stack FromInfixStringToPostfixStack(double input_x) {
    while (*it_symbol_ != '\0') {
      CreateLexem();
      if (temp_lexem_.type == NUM) {
        output_stack_.push(temp_lexem_);
      } else if (temp_lexem_.type == X) {
        temp_lexem_.value = input_x;
        temp_lexem_.type = NUM;
        output_stack_.push(temp_lexem_);
      } else if (temp_lexem_.type == UN_ADD || temp_lexem_.type == UN_SUB) {
        temp_stack_.push(temp_lexem_);
        output_stack_.push({0.0, NUM, NONE, NONE});
      } else if (temp_lexem_.type == LEFT_BRACKET || temp_lexem_.func) {
        temp_stack_.push(temp_lexem_);
      } else if (temp_lexem_.type == RIGHT_BRACKET) {
        PushToOutputStackFromTempStackInBrackets();
      } else {
        PushToOutputStackFromTempStackRemainingOpers();
      }
      ++it_symbol_;
    }
    while (!temp_stack_.empty()) {
      PushToOutputPopFromTemp();
    }
    return output_stack_;
  };

 private:
  void CreateLexem() {
    temp_lexem_ = {NONE, NONE, NONE, NONE};
    if (validator_.IsNumeric(*it_symbol_)) {
      temp_lexem_.value = GetNumFromInputString();
      SetTypeAndPriorityInLexem(NUM, ZERO);
    } else if (*it_symbol_ == '(') {
      SetTypeAndPriorityInLexem(LEFT_BRACKET, FOURTH);
    } else if (*it_symbol_ == ')') {
      SetTypeAndPriorityInLexem(RIGHT_BRACKET, FOURTH);
    } else if (*it_symbol_ == 'x' || *it_symbol_ == 'X') {
      SetTypeAndPriorityInLexem(X, ZERO);
    } else if (*it_symbol_ == '^') {
      SetTypeAndPriorityInLexem(EXP, FIRST);
    } else if (*it_symbol_ == '*') {
      SetTypeAndPriorityInLexem(MULT, FIRST);
    } else if (*it_symbol_ == '/') {
      SetTypeAndPriorityInLexem(DIV, FIRST);
    } else if (*it_symbol_ == '%' || *it_symbol_ == 'm') {
      if (*it_symbol_ == 'm') {
        it_symbol_ += validator_.kLenMod - 1;
      }
      SetTypeAndPriorityInLexem(MOD, ZERO);
    } else if (*it_symbol_ == '+') {
      SetTypeAndPriorityInLexem(CreatePlusOrMinus(ADD, UN_ADD), SECOND);
    } else if (*it_symbol_ == '-') {
      SetTypeAndPriorityInLexem(CreatePlusOrMinus(SUB, UN_SUB), SECOND);
    } else {
      SetTypeAndPriorityInLexem(FUNC, FOURTH);
      temp_lexem_.func = validator_.GetFunction(it_symbol_);
    }
  };

  double GetNumFromInputString() {
    double result = 0.0;
    std::regex numberRegex(R"([-+]?\d+(\.\d+)?([eE][-+]?\d+)?)");
    std::string tempString(it_symbol_, it_symbol_end_);
    std::smatch match;
    if (std::regex_search(tempString, match, numberRegex)) {
      it_symbol_ += (match.length() - 1);
      result = std::stod(match.str());
    }
    return result;
  }

  int CreatePlusOrMinus(int sign, int unary_sign) {
    if (it_symbol_ != it_symbol_begin_ && *(it_symbol_ - 1) == '(') {
      return unary_sign;
    } else {
      return sign;
    }
  };

  void PushToOutputStackFromTempStackRemainingOpers() {
    while (!temp_stack_.empty() &&
           (temp_stack_.top().priority < temp_lexem_.priority ||
            (temp_stack_.top().priority == temp_lexem_.priority &&
             temp_lexem_.type != EXP))) {
      PushToOutputPopFromTemp();
    }
    temp_stack_.push(temp_lexem_);
  };

  void SetTypeAndPriorityInLexem(int type, int priority) {
    temp_lexem_.type = type;
    temp_lexem_.priority = priority;
  };

  void PushToOutputStackFromTempStackInBrackets() {
    while (!temp_stack_.empty() && temp_stack_.top().type != LEFT_BRACKET) {
      PushToOutputPopFromTemp();
    }
    if (!temp_stack_.empty() && temp_stack_.top().type == LEFT_BRACKET) {
      temp_stack_.pop();
    }
    if (!temp_stack_.empty() && temp_stack_.top().func) {
      PushToOutputPopFromTemp();
    }
  };

  void PushToOutputPopFromTemp() {
    output_stack_.push(temp_stack_.top());
    temp_stack_.pop();
  }

 private:
  StringIterator it_symbol_;
  StringIterator it_symbol_begin_;
  StringIterator it_symbol_end_;
  Stack temp_stack_;
  Stack output_stack_;
  Lexem temp_lexem_ = {NONE, NONE, NONE, NONE};
  ValidatorModel validator_;
};

}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_SRC_MODEL_CONVERTER_MODEL_H_
