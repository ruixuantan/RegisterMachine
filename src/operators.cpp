#include "operators.hpp"
#include "register.hpp"

#include <string>
#include <memory>
#include <utility>

using namespace operators;
using namespace registers;

Variable::Variable(size_t value, bool isRegister)
  : value{ value }, isRegister{ isRegister } {}

size_t Variable::eval(const Register& r) const {
  if (isRegister) {
    return r.getRegister(value); 
  } else {
    return value;
  }
}

bool operators::operator== (const Variable& v1, const Variable& v2) {
  return v1.value == v2.value && v1.isRegister == v2.isRegister;
}

BinaryOperator::BinaryOperator(std::string keyword, Variable lhs, Variable rhs)
  : keyword{std::move(keyword)}, lhs{lhs}, rhs{rhs} {}

std::string BinaryOperator::getKeyword() const {
  return keyword;
}

Variable BinaryOperator::getLhs() const {
  return lhs;
}

Variable BinaryOperator::getRhs() const {
  return rhs;
}

bool operators::operator== (const BinaryOperator& o1, const BinaryOperator& o2) {
  return o1.keyword == o2.keyword && o1.lhs == o2.lhs && o1.rhs == o2.rhs;
}

BinaryOperator::~BinaryOperator() = default;

const std::string AddOperator::keyword { "+" };

AddOperator::AddOperator(Variable lhs, Variable rhs)
  : BinaryOperator{AddOperator::keyword, lhs, rhs} {}

size_t AddOperator::eval(const Register& r) const {
  return lhs.eval(r) + rhs.eval(r);
}

AddOperator::~AddOperator() = default;

const std::string SubtractOperator::keyword { "-" };

SubtractOperator::SubtractOperator(Variable lhs, Variable rhs)
  : BinaryOperator{SubtractOperator::keyword, lhs, rhs} {}

size_t SubtractOperator::eval(const Register& r) const {
  return lhs.eval(r) - rhs.eval(r);
}

SubtractOperator::~SubtractOperator() = default;

ComparisonOperator::ComparisonOperator(std::string keyword, Variable lhs, Variable rhs)
  : keyword{std::move(keyword)}, lhs{lhs}, rhs{rhs} {}

std::string ComparisonOperator::getKeyword() const {
  return keyword;
}

ComparisonOperator::~ComparisonOperator() = default;

const std::string EqualityOperator::keyword { "==" };

EqualityOperator::EqualityOperator(Variable lhs, Variable rhs)
  : ComparisonOperator{EqualityOperator::keyword, lhs, rhs} {}

bool EqualityOperator::eval(const Register& r) const {
  return lhs.eval(r) == rhs.eval(r);
}

EqualityOperator::~EqualityOperator() = default;

const std::string LessThanOperator::keyword { "<" };

LessThanOperator::LessThanOperator(Variable lhs, Variable rhs)
  : ComparisonOperator{LessThanOperator::keyword, lhs, rhs} {}

bool LessThanOperator::eval(const Register& r) const {
  return lhs.eval(r) < rhs.eval(r);
}

LessThanOperator::~LessThanOperator() = default;

Operator::Operator(std::string keyword)
  : keyword{std::move(keyword)} {}

std::string Operator::getKeyword() const {
  return keyword;
}

Operator::~Operator() = default;

const std::string AssignmentOperator::keyword = "=";

AssignmentOperator::AssignmentOperator(size_t regNumber, std::unique_ptr<BinaryOperator> op)
  : Operator{AssignmentOperator::keyword}, regNumber{regNumber}, op{std::move(op)} {}

size_t AssignmentOperator::exec(size_t programCounter, Register& r) const {
  r.setRegister(op->eval(r), regNumber);
  return ++programCounter;
}

AssignmentOperator::~AssignmentOperator() = default;

const std::string GotoOperator::keyword { "goto" };

GotoOperator::GotoOperator(size_t lineNumber)
  : Operator{GotoOperator::keyword}, lineNumber{lineNumber} {}

size_t GotoOperator::exec(size_t programCounter, Register& r) const {
  return lineNumber;
}

GotoOperator::~GotoOperator() = default;

const std::string ReturnOperator::keyword { "return" };

ReturnOperator::ReturnOperator(size_t returnRegNumber)
  : Operator{ReturnOperator::keyword}, returnRegNumber{returnRegNumber} {}

size_t ReturnOperator::exec(size_t programCounter, Register& r) const {
  return returnRegNumber;
}

ReturnOperator::~ReturnOperator() = default;

IfOperator::IfOperator(size_t lineNumber, std::unique_ptr<ComparisonOperator> op)
  : Operator{IfOperator::keyword}, lineNumber{lineNumber}, op{std::move(op)} {}
 
const std::string IfOperator::keyword { "if" };

size_t IfOperator::exec(size_t programCounter, Register& r) const {
  if (op->eval(r)) {
    return lineNumber - 1;
  } else {
    return ++programCounter;
  }
}

IfOperator::~IfOperator() = default;
