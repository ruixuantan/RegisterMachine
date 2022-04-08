#include "operators.hpp"
#include "register.hpp"

#include <string>
#include <memory>
#include <utility>

using namespace operators;
using namespace registers;

Variable::Variable(int value, bool isRegister)
  : value{ value }, isRegister{ isRegister } {}

int Variable::eval(const Register& r) const {
  if (this->isRegister) {
    return r.getRegister(this->value); 
  } else {
    return this->value;
  }
}

bool operators::operator== (const Variable& v1, const Variable& v2) {
  return v1.value == v2.value && v1.isRegister == v2.isRegister;
}

BinaryOperator::BinaryOperator(std::string keyword, Variable lhs, Variable rhs)
  : keyword{std::move(keyword)}, lhs{lhs}, rhs{rhs} {}

std::string BinaryOperator::getKeyword() const {
  return this->keyword;
}

Variable BinaryOperator::getLhs() const {
  return this->lhs;
}

Variable BinaryOperator::getRhs() const {
  return this->rhs;
}

bool operators::operator== (const BinaryOperator& o1, const BinaryOperator& o2) {
  return o1.keyword == o2.keyword && o1.lhs == o2.lhs && o1.rhs == o2.rhs;
}

BinaryOperator::~BinaryOperator() = default;;

const std::string AddOperator::keyword { "+" };

AddOperator::AddOperator(Variable lhs, Variable rhs)
  : BinaryOperator{AddOperator::keyword, lhs, rhs} {};

int AddOperator::eval(const Register& r) const {
  return this->lhs.eval(r) + this->rhs.eval(r);
}

AddOperator::~AddOperator() = default;;

const std::string SubtractOperator::keyword { "-" };

SubtractOperator::SubtractOperator(Variable lhs, Variable rhs)
  : BinaryOperator{SubtractOperator::keyword, lhs, rhs} {};

int SubtractOperator::eval(const Register& r) const {
  return this->lhs.eval(r) - this->rhs.eval(r);
}

SubtractOperator::~SubtractOperator() = default;;

ComparisonOperator::ComparisonOperator(std::string keyword, Variable lhs, Variable rhs)
  : keyword{keyword}, lhs{lhs}, rhs{rhs} {}

std::string ComparisonOperator::getKeyword() const {
  return this->keyword;
}

ComparisonOperator::~ComparisonOperator() = default;

const std::string EqualityOperator::keyword { "==" };

EqualityOperator::EqualityOperator(Variable lhs, Variable rhs)
  : ComparisonOperator{EqualityOperator::keyword, lhs, rhs} {};

bool EqualityOperator::eval(const Register& r) const {
  return this->lhs.eval(r) == this->rhs.eval(r);
}

EqualityOperator::~EqualityOperator() = default;

const std::string LessThanOperator::keyword { "<" };

LessThanOperator::LessThanOperator(Variable lhs, Variable rhs)
  : ComparisonOperator{LessThanOperator::keyword, lhs, rhs} {}

bool LessThanOperator::eval(const Register& r) const {
  return this->lhs.eval(r) < this->rhs.eval(r);
}

LessThanOperator::~LessThanOperator() = default;

Operator::Operator(std::string keyword)
  : keyword{std::move(keyword)} {}

std::string Operator::getKeyword() const {
  return this->keyword;
}

Operator::~Operator() = default;

const std::string AssignmentOperator::keyword = "=";

AssignmentOperator::AssignmentOperator(int regNumber, std::unique_ptr<BinaryOperator> op)
  : Operator{AssignmentOperator::keyword}, regNumber{regNumber}, op{std::move(op)} {};

int AssignmentOperator::exec(int programCounter, Register& r) const {
  r.setRegister(op->eval(r), this->regNumber);
  return ++programCounter;
}

AssignmentOperator::~AssignmentOperator() = default;

const std::string GotoOperator::keyword { "goto" };

GotoOperator::GotoOperator(int lineNumber)
  : Operator{GotoOperator::keyword}, lineNumber{lineNumber} {};

int GotoOperator::exec(int programCounter, Register& r) const {
  return this->lineNumber;
}

GotoOperator::~GotoOperator() {}

const std::string ReturnOperator::keyword { "return" };

ReturnOperator::ReturnOperator(int returnRegNumber)
  : Operator{ReturnOperator::keyword}, returnRegNumber{returnRegNumber} {}

int ReturnOperator::exec(int programCounter, Register& r) const {
  return this->returnRegNumber;
}

ReturnOperator::~ReturnOperator() = default;

IfOperator::IfOperator(int lineNumber, std::unique_ptr<ComparisonOperator> op)
  : Operator{IfOperator::keyword}, lineNumber{lineNumber}, op{std::move(op)} {};
 
const std::string IfOperator::keyword { "if" };

int IfOperator::exec(int programCounter, Register& r) const {
  if (this->op->eval(r)) {
    return this->lineNumber - 1;
  } else {
    return ++programCounter;
  }
}

IfOperator::~IfOperator() = default;
