#ifndef CPP3_SMARTCALC_V2_SRC_MODEL_VALIDATOR_H_
#define CPP3_SMARTCALC_V2_SRC_MODEL_VALIDATOR_H_

#include <algorithm>
#include <array>
#include <string>

namespace s21 {

enum ErrorCode {
  OK,
  INCORRECT_STRING,
  DIV_ON_ZERO,
  NEGATIVE_SQRT,
  INCORRECT_COORDINATE_SYSTEM
};

class ValidatorModel {
 public:
  using StringIterator = std::string::const_iterator;

  static const int kLenMod = 3;
  static const int kBeginIndex = 0;

 private:
  static const int kNumOfFuncs_ = 9;
  const std::array<std::string, kNumOfFuncs_> kFuncs = {
      "cos", "sin", "tan", "acos", "asin", "atan", "sqrt", "ln", "log"};

 public:
  ValidatorModel(){};

  ValidatorModel(std::string& input) {
    it_symbol_ = input.begin();
    input.erase(std::remove(input.begin(), input.end(), ' '), input.end());
    input.push_back('\0');
    it_symbol_ = input.begin();
    it_symbol_begin_ = it_symbol_;
    left_bracket_ = 0;
    right_bracket_ = 0;
    dot_count_ = 0;
  }

  ValidatorModel& operator=(ValidatorModel&& other) noexcept {
    if (this != &other) {
      it_symbol_ = std::move(other.it_symbol_);
      it_symbol_begin_ = std::move(other.it_symbol_begin_);
      error_ = other.error_;
      left_bracket_ = other.left_bracket_;
      right_bracket_ = other.right_bracket_;
      dot_count_ = other.dot_count_;
    }
    return *this;
  }

  int Validate() {
    error_ = OK;

    while (*it_symbol_ != '\0' && error_ == OK) {
      error_ = INCORRECT_STRING;
      ProcessCharacter();
      ++it_symbol_;
    }

    if (left_bracket_ - right_bracket_ != 0) {
      error_ = INCORRECT_STRING;
    }

    return error_;
  }

  int GetFunction(StringIterator& it_symbol) {
    int flag = true;
    for (int func_number = kBeginIndex; func_number < kNumOfFuncs_ && flag;
         ++func_number) {
      StringIterator temp_it = it_symbol;
      int letter = 0;
      for (; kFuncs[func_number][letter] != '\0' && flag; ++letter) {
        if (kFuncs[func_number][letter] != *temp_it) {
          flag = false;
        }
        ++temp_it;
      }
      if (flag) {
        it_symbol = temp_it - 1;
        return func_number + 1;
      } else {
        flag = true;
      }
    }
    return 0;
  };

  bool IsMod(StringIterator& it_symbol) {
    std::string mod_str = "mod";
    for (int i = kBeginIndex; mod_str[i]; i++) {
      if (mod_str[i] != *it_symbol) {
        return false;
      }
      ++it_symbol;
    }
    --it_symbol;
    return true;
  }

  bool IsNumeric(char symbol) { return (symbol >= '0' && symbol <= '9'); }

  bool IsAddSub(char symbol) { return (symbol == '+' || symbol == '-'); }

  bool IsMultDiv(char symbol) {
    return (symbol == '*' || symbol == '/' || symbol == '^');
  }

  bool IsOperator(char symbol) {
    return (IsMultDiv(symbol) || IsAddSub(symbol));
  }

  bool IsE(char symbol) { return (symbol == 'e' || symbol == 'E'); }

  bool IsX(char symbol) { return (symbol == 'x'); }

  bool IsBrackets(char symbol) { return (symbol == '(' || symbol == ')'); }

  bool IsDot(char symbol) { return (symbol == '.'); }

  bool IsChar(char symbol) { return (symbol >= 'a' && symbol <= 'z'); }

  bool IsSpace(char symbol) { return (symbol == ' '); }

  bool IsNumericOrLeftBracket(char symbol) {
    return (IsNumeric(symbol) || symbol == '(');
  }

  bool IsNumericOrRightBracket(char symbol) {
    return (IsNumeric(symbol) || symbol == ')');
  }

 private:
  void ProcessCharacter() {
    if (IsBrackets(*it_symbol_)) {
      ProcessBrackets();
    } else if (IsE(*it_symbol_)) {
      ProcessE();
    } else if (IsX(*it_symbol_)) {
      ProcessX();
    } else if (IsChar(*it_symbol_)) {
      ProcessChar();
    } else if (IsDot(*it_symbol_)) {
      ProcessDot();
    } else if (IsAddSub(*it_symbol_)) {
      ProcessAddSub();
    } else if (IsMultDiv(*it_symbol_)) {
      ProcessMultDiv();
    } else if (IsNumeric(*it_symbol_)) {
      error_ = OK;
    }
  }

  void ProcessBrackets() {
    if (*it_symbol_ == '(') {
      left_bracket_++;
    } else {
      right_bracket_++;
    }
    if (*it_symbol_ == '(' && *(it_symbol_ + 1) == ')') {
      error_ = INCORRECT_STRING;
    } else if (*(it_symbol_) == ')' && IsNumeric(*(it_symbol_ + 1))) {
      error_ = INCORRECT_STRING;
    } else {
      error_ = OK;
    }
  }

  void ProcessChar() {
    char symbol = '*';
    if (it_symbol_ != it_symbol_begin_) {
      symbol = *(it_symbol_ - 1);
    }
    if (GetFunction(it_symbol_)) {
      if ((IsOperator(symbol) || symbol == '(') && *(it_symbol_ + 1) == '(') {
        error_ = OK;
      }
    } else if (IsMod(it_symbol_) && it_symbol_ != it_symbol_begin_) {
      if (IsNumericOrRightBracket(*(it_symbol_ - kLenMod)) &&
          IsNumericOrLeftBracket(*(it_symbol_ + 1))) {
        error_ = OK;
      }
    }
  }

  void ProcessDot() {
    if (it_symbol_ != it_symbol_begin_ && dot_count_ == 0) {
      if (IsNumeric(*(it_symbol_ - 1)) || IsNumeric(*(it_symbol_ + 1))) {
        dot_count_++;
        error_ = OK;
      }
    }
  }

  void ProcessAddSub() {
    if ((IsNumericOrLeftBracket(*(it_symbol_ + 1)) ||
         IsChar(*(it_symbol_ + 1)))) {
      error_ = OK;
    }
    if (dot_count_) {
      dot_count_--;
    }
  }

  void ProcessMultDiv() {
    if (it_symbol_ != it_symbol_begin_) {
      if (IsNumericOrLeftBracket(*(it_symbol_ + 1)) ||
          IsChar(*(it_symbol_ + 1))) {
        error_ = OK;
      }
      if (dot_count_) {
        dot_count_--;
      }
    }
  }

  void ProcessX() {
    if (it_symbol_ != it_symbol_begin_) {
      if (IsOperator(*(it_symbol_ - 1)) || *(it_symbol_ - 1) == '(') {
        error_ = OK;
      }
    } else {
      if (IsOperator(*(it_symbol_ + 1)) || *(it_symbol_ + 1) == ')' || *(it_symbol_ + 1) == 'm') {
        error_ = OK;
      }
    }
  }

  void ProcessE() {
    if (it_symbol_ != it_symbol_begin_) {
      if (IsNumeric(*(it_symbol_ - 1)) && IsOperator(*(it_symbol_ + 1))) {
        error_ = OK;
      }
    }
  }

 private:
  StringIterator it_symbol_;
  StringIterator it_symbol_begin_;
  int error_ = 0;
  int left_bracket_ = 0;
  int right_bracket_ = 0;
  int dot_count_ = 0;
};

}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_SRC_MODEL_VALIDATOR_H_
