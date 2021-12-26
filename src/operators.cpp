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

bool operator== (const Variable& v1, const Variable& v2) {
  return v1.value == v2.value && v1.isRegister == v2.isRegister;
}

BinaryOperator::BinaryOperator(std::string keyword, Variable lhs, Variable rhs)
  : keyword{keyword}, lhs{lhs}, rhs{rhs} {}

std::string BinaryOperator::getKeyword() const {
  return this->keyword;
}

Variable BinaryOperator::getLhs() const {
  return this->lhs;
}

Variable BinaryOperator::getRhs() const {
  return this->rhs;
}

bool operator== (const BinaryOperator& o1, const BinaryOperator& o2) {
  return o1.keyword == o2.keyword && o1.lhs == o2.lhs && o1.rhs == o2.rhs;
}

BinaryOperator::~BinaryOperator() {};

std::string AddOperator::keyword { "+" };

AddOperator::AddOperator(Variable lhs, Variable rhs)
  : BinaryOperator{AddOperator::keyword, lhs, rhs} {};

int AddOperator::eval(Register &r) const {
  return this->lhs.eval(r) + this->rhs.eval(r);
}

AddOperator::~AddOperator() {};

std::string SubtractOperator::keyword { "-" };

SubtractOperator::SubtractOperator(Variable lhs, Variable rhs)
  : BinaryOperator{SubtractOperator::keyword, lhs, rhs} {};

int SubtractOperator::eval(Register &r) const {
  return this->lhs.eval(r) - this->rhs.eval(r);
}

SubtractOperator::~SubtractOperator() {};

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

Operator::~Operator() {}

std::string AssignmentOperator::keyword = "=";

AssignmentOperator::AssignmentOperator(int regNumber, std::unique_ptr<BinaryOperator> op)
  : Operator{AssignmentOperator::keyword}, regNumber{regNumber}, op{std::move(op)} {};

int AssignmentOperator::exec(int programCounter, Register &r) const {
  r.setRegister(op->eval(r), this->regNumber);
  return ++programCounter;
}

AssignmentOperator::~AssignmentOperator() {}

std::string GotoOperator::keyword { "goto" };

GotoOperator::GotoOperator(int lineNumber)
  : Operator{GotoOperator::keyword}, lineNumber{lineNumber} {};

int GotoOperator::exec(int programCounter, Register &r) const {
  return this->lineNumber;
}

GotoOperator::~GotoOperator() {}

std::string ReturnOperator::keyword { "return" };

ReturnOperator::ReturnOperator(int returnRegNumber)
  : Operator{ReturnOperator::keyword}, returnRegNumber{returnRegNumber} {}

int ReturnOperator::exec(int programCounter, Register &r) const {
  return this->returnRegNumber;
}

ReturnOperator::~ReturnOperator() {}
