#include <gtest/gtest.h>

#include "../models/calculator_model.h"

class CalcModelTest : public ::testing::Test {
 protected:
  s21::CalcModel calculator_test;
};

TEST_F(CalcModelTest, TestValidator1) {
  std::string expression = "2+3**4";
  double x_value = 0.0;
  double result = calculator_test.CalculateResult(expression, x_value);
  int error = calculator_test.GetError();
  EXPECT_EQ(error, s21::INCORRECT_STRING);
  EXPECT_NEAR(result, 0.0, 0.000001);
}

TEST_F(CalcModelTest, TestValidatorRightBracket) {
  std::string expression = "(1223))";
  double x_value = 0.0;
  double result = calculator_test.CalculateResult(expression, x_value);
  int error = calculator_test.GetError();
  EXPECT_EQ(error, s21::INCORRECT_STRING);
  EXPECT_NEAR(result, 0.0, 0.000001);
}

TEST_F(CalcModelTest, TestValidatorLeftBracket) {
  std::string expression = "((12+23";
  double x_value = 0.0;
  double result = calculator_test.CalculateResult(expression, x_value);
  int error = calculator_test.GetError();
  EXPECT_EQ(error, s21::INCORRECT_STRING);
  EXPECT_NEAR(result, 0.0, 0.000001);
}

TEST_F(CalcModelTest, TestValidatorTwoDots) {
  std::string expression = ".12+23.";
  double x_value = 0.0;
  double result = calculator_test.CalculateResult(expression, x_value);
  int error = calculator_test.GetError();
  EXPECT_EQ(error, s21::INCORRECT_STRING);
  EXPECT_NEAR(result, 0.0, 0.000001);
}

TEST_F(CalcModelTest, TestValidatorChars) {
  std::string expression = "sdf12+23";
  double x_value = 0.0;
  double result = calculator_test.CalculateResult(expression, x_value);
  int error = calculator_test.GetError();
  EXPECT_EQ(error, s21::INCORRECT_STRING);
  EXPECT_NEAR(result, 0.0, 0.000001);
}

TEST_F(CalcModelTest, TestValidatorWrongMod) {
  std::string expression = "mod12+23";
  double x_value = 0.0;
  double result = calculator_test.CalculateResult(expression, x_value);
  int error = calculator_test.GetError();
  EXPECT_EQ(error, s21::INCORRECT_STRING);
  EXPECT_NEAR(result, 0.0, 0.000001);
}

TEST_F(CalcModelTest, TestValidatorWrongSin) {
  std::string expression = "sin12+23";
  double x_value = 0.0;
  double result = calculator_test.CalculateResult(expression, x_value);
  int error = calculator_test.GetError();
  EXPECT_EQ(error, s21::INCORRECT_STRING);
  EXPECT_NEAR(result, 0.0, 0.000001);
}

TEST_F(CalcModelTest, TestValidatorWrongE) {
  std::string expression = "2eeeee-2";
  double x_value = 0.0;
  double result = calculator_test.CalculateResult(expression, x_value);
  int error = calculator_test.GetError();
  EXPECT_EQ(error, s21::INCORRECT_STRING);
  EXPECT_NEAR(result, 0.0, 0.000001);
}

TEST_F(CalcModelTest, TestValidatorOperatorWithoutNum) {
  std::string expression = "*(-4)";
  double x_value = 0.0;
  double result = calculator_test.CalculateResult(expression, x_value);
  int error = calculator_test.GetError();
  EXPECT_EQ(error, s21::INCORRECT_STRING);
  EXPECT_NEAR(result, 0.0, 0.000001);
}

TEST_F(CalcModelTest, TestValidatorEmptyBrackets) {
  std::string expression = "()";
  double x_value = 0.0;
  double result = calculator_test.CalculateResult(expression, x_value);
  int error = calculator_test.GetError();
  EXPECT_EQ(error, s21::INCORRECT_STRING);
  EXPECT_NEAR(result, 0.0, 0.000001);
}

TEST_F(CalcModelTest, TestValidatorTwoFunction) {
  std::string expression = "cossin()";
  double x_value = 0.0;
  double result = calculator_test.CalculateResult(expression, x_value);
  int error = calculator_test.GetError();
  EXPECT_EQ(error, s21::INCORRECT_STRING);
  EXPECT_NEAR(result, 0.0, 0.000001);
}

TEST_F(CalcModelTest, TestValidatorFuncWithDot) {
  std::string expression = "cos(.)";
  double x_value = 0.0;
  double result = calculator_test.CalculateResult(expression, x_value);
  int error = calculator_test.GetError();
  EXPECT_EQ(error, s21::INCORRECT_STRING);
  EXPECT_NEAR(result, 0.0, 0.000001);
}

TEST_F(CalcModelTest, TestCalculation1) {
  std::string expression = "2 * log(5 - 3 + 5) / 2 + 2 * ln(5 * 6) * (-2 / 5)";
  double x_value = 0.0;
  double result = calculator_test.CalculateResult(expression, x_value);
  int error = calculator_test.GetError();
  EXPECT_EQ(error, s21::OK);
  double exp = -1.875859;
  EXPECT_NEAR(result, exp, 0.000001);
}

TEST_F(CalcModelTest, TestCalculation2) {
  std::string expression = "2^2^ tan(ln(cos(si n (sqrt(64*2-2-2)))))";
  double x_value = 0.0;
  double result = calculator_test.CalculateResult(expression, x_value);
  int error = calculator_test.GetError();
  EXPECT_EQ(error, s21::OK);
  double exp = 1.539031;
  EXPECT_NEAR(result, exp, 0.000001);
}

TEST_F(CalcModelTest, TestCalculation3) {
  std::string expression = "3243 / 21 /12 /1 * 2 * 1 * 2^2^3";
  double x_value = 0.0;
  double result = calculator_test.CalculateResult(expression, x_value);
  int error = calculator_test.GetError();
  EXPECT_EQ(error, s21::OK);
  double exp = 6588.95238;
  EXPECT_NEAR(result, exp, 0.000001);
}

TEST_F(CalcModelTest, TestCalculation4) {
  std::string expression =
      "56 * ln(1.248369) * sqrt(1) - 24 - 32 - 23 - 2 / 21 + 3.2754893";
  double x_value = 0.0;
  double result = calculator_test.CalculateResult(expression, x_value);
  int error = calculator_test.GetError();
  EXPECT_EQ(error, s21::OK);
  double exp = -63.396826;
  EXPECT_NEAR(result, exp, 0.000001);
}

TEST_F(CalcModelTest, TestCalculation5) {
  std::string expression = "2E-24 * 45 - sqrt(36)";
  double x_value = 0.0;
  double result = calculator_test.CalculateResult(expression, x_value);
  int error = calculator_test.GetError();
  EXPECT_EQ(error, s21::OK);
  double exp = -6;
  EXPECT_NEAR(result, exp, 0.000001);
}
TEST_F(CalcModelTest, TestCalculation6) {
  std::string expression =
      "-1+(sqrt(3.141592^log(5-3.141592))+log(55/(2+3.141592)))";
  double x_value = 0.0;
  double result = calculator_test.CalculateResult(expression, x_value);
  int error = calculator_test.GetError();
  EXPECT_EQ(error, s21::OK);
  double exp = 1.195811;
  EXPECT_NEAR(result, exp, 0.000001);
}

TEST_F(CalcModelTest, TestCalculation7) {
  std::string expression = "(3.4)mod3";
  double x_value = 0.0;
  double result = calculator_test.CalculateResult(expression, x_value);
  int error = calculator_test.GetError();
  EXPECT_EQ(error, s21::OK);
  double exp = 0.399999;
  EXPECT_NEAR(result, exp, 0.000001);
}

TEST_F(CalcModelTest, TestCalculation8) {
  std::string expression = "1-(-1)";
  double x_value = 0.0;
  double result = calculator_test.CalculateResult(expression, x_value);
  int error = calculator_test.GetError();
  EXPECT_EQ(error, s21::OK);
  double exp = 2.0;
  EXPECT_NEAR(result, exp, 0.000001);
}

TEST_F(CalcModelTest, TestCalculation9) {
  std::string expression = "+1-(-1)";
  double x_value = 0.0;
  double result = calculator_test.CalculateResult(expression, x_value);
  int error = calculator_test.GetError();
  EXPECT_EQ(error, s21::OK);
  double exp = 2.0;
  EXPECT_NEAR(result, exp, 0.000001);
}

TEST_F(CalcModelTest, TestCalculation10) {
  std::string expression = "";
  double x_value = 0.0;
  double result = calculator_test.CalculateResult(expression, x_value);
  int error = calculator_test.GetError();
  EXPECT_EQ(error, s21::OK);
  double exp = 0.0;
  EXPECT_NEAR(result, exp, 0.000001);
}

TEST_F(CalcModelTest, TestCalculation11) {
  std::string expression = "2384752983746098273";
  double x_value = 0.0;
  double result = calculator_test.CalculateResult(expression, x_value);
  int error = calculator_test.GetError();
  EXPECT_EQ(error, s21::OK);
  double exp = 2384752983746098273;
  EXPECT_NEAR(result, exp, 0.000001);
}

TEST_F(CalcModelTest, TestCalculation12) {
  std::string expression = "234.234+2";
  double x_value = 0.0;
  double result = calculator_test.CalculateResult(expression, x_value);
  int error = calculator_test.GetError();
  EXPECT_EQ(error, s21::OK);
  double exp = 236.234;
  EXPECT_NEAR(result, exp, 0.000001);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
