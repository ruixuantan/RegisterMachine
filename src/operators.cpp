#include "operators.hpp"
#include "register.hpp"

#include <string>
#include <iostream>

BinaryOperator::BinaryOperator(std::string keyword, int lhsToken, int rhsToken)
  : keyword{keyword}, lhsToken{lhsToken}, rhsToken{rhsToken} {}

std::string BinaryOperator::getKeyword() const {
  return this->keyword;
}

std::string AddOperator::keyword { "+" };

AddOperator::AddOperator(int lhsToken, int rhsToken)
  : BinaryOperator{AddOperator::keyword, lhsToken, rhsToken} {};

int AddOperator::eval() const {
  return this->lhsToken + this->rhsToken;
}

std::string SubtractOperator::keyword { "-" };

SubtractOperator::SubtractOperator(int lhsToken, int rhsToken)
  : BinaryOperator{"-", lhsToken, rhsToken} {};

int SubtractOperator::eval() const {
  return this->lhsToken + this->rhsToken;
}

ComparisonOperator::ComparisonOperator(std::string keyword, int lhsToken, int rhsToken)
  : keyword{keyword}, lhsToken{lhsToken}, rhsToken{rhsToken} {}

std::string ComparisonOperator::getKeyword() const {
  return this->keyword;
}

std::string EqualityOperator::keyword { "==" };

EqualityOperator::EqualityOperator(int lhsToken, int rhsToken)
  : ComparisonOperator{EqualityOperator::keyword, lhsToken, rhsToken} {};

bool EqualityOperator::eval() const {
  return this->lhsToken == this->rhsToken;
}

std::string LessThanOperator::keyword { "<" };

LessThanOperator::LessThanOperator(int lhsToken, int rhsToken)
  : ComparisonOperator{LessThanOperator::keyword, lhsToken, rhsToken} {}

bool LessThanOperator::eval() const {
  return this->lhsToken < this->rhsToken;
}

Operator::Operator(std::string keyword)
  : keyword{keyword} {}

std::string Operator::getKeyword() const {
  return this->keyword;
}

int Operator::exec(int programCounter, Register &r) const {
  return TERMINATE_LINE_NUMBER;
}

std::string AssignmentOperator::keyword = "=";

AssignmentOperator::AssignmentOperator(int regNumber, int value)
  : Operator{AssignmentOperator::keyword}, regNumber{regNumber}, value{value} {};

int AssignmentOperator::exec(int programCounter, Register &r) const {
  r.setRegister(this->value, this->regNumber);
  return ++programCounter;
}

std::string GotoOperator::keyword { "goto" };

GotoOperator::GotoOperator(int lineNumber)
  : Operator{GotoOperator::keyword}, lineNumber{lineNumber} {};

int GotoOperator::exec(int programCounter, Register &r) const {
  return this->lineNumber;
}

std::string ReturnOperator::keyword { "return" };

ReturnOperator::ReturnOperator(int returnRegNumber)
  : Operator{ReturnOperator::keyword}, returnRegNumber{returnRegNumber} {}

int ReturnOperator::exec(int programCounter, Register &r) const {
  return this->returnRegNumber;
}