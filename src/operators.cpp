#include "operators.hpp"
#include "register.hpp"

#include <string>
#include <iostream>

Variable::Variable()
  : value{ 0 }, isRegister{ false } {}

Variable::Variable(int value, bool isRegister)
  : value{ value }, isRegister{ isRegister } {}

int Variable::eval(Register &r) const {
  if (this->isRegister) {
    return r.getRegister(this->value); 
  } else {
    return this->value;
  }
}

BinaryOperator::BinaryOperator(std::string keyword, Variable lhs, Variable rhs)
  : keyword{keyword}, lhs{lhs}, rhs{rhs} {}

std::string BinaryOperator::getKeyword() const {
  return this->keyword;
}

int BinaryOperator::eval(Register &r) const {
  return 0;
}

std::string AddOperator::keyword { "+" };

AddOperator::AddOperator(Variable lhs, Variable rhs)
  : BinaryOperator{AddOperator::keyword, lhs, rhs} {};

int AddOperator::eval(Register &r) const {
  return this->lhs.eval(r) + this->rhs.eval(r);
}

std::string SubtractOperator::keyword { "-" };

SubtractOperator::SubtractOperator(Variable lhs, Variable rhs)
  : BinaryOperator{SubtractOperator::keyword, lhs, rhs} {};

int SubtractOperator::eval(Register &r) const {
  return this->lhs.eval(r) - this->rhs.eval(r);
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

AssignmentOperator::AssignmentOperator(int regNumber, std::unique_ptr<BinaryOperator> op)
  : Operator{AssignmentOperator::keyword}, regNumber{regNumber}, op{std::move(op)} {};

int AssignmentOperator::exec(int programCounter, Register &r) const {
  r.setRegister(op->eval(r), this->regNumber);
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