#ifndef OPERATOR_H
#define OPERATOR_H

#include "register.hpp"

#include <string>
#include <memory>

namespace operators {
class Variable {
 private:
  size_t value;
  bool isRegister;
 public:
  explicit Variable(size_t value = 0, bool isRegister = false);
  size_t eval(const registers::Register &r) const;
  friend bool operator==(const Variable &v1, const Variable &v2);
};

class BinaryOperator {
 protected:
  std::string keyword;
  Variable lhs;
  Variable rhs;

 public:
  BinaryOperator(std::string keyword, Variable lhs, Variable rhs);
  std::string getKeyword() const;
  Variable getLhs() const;
  Variable getRhs() const;
  friend bool operator==(const BinaryOperator &o1, const BinaryOperator &o2);
  virtual size_t eval(const registers::Register &r) const = 0;
  virtual ~BinaryOperator();
};

class AddOperator : public BinaryOperator {
 public:
  AddOperator(Variable lhs, Variable rhs);
  static std::string keyword();
  size_t eval(const registers::Register &r) const override;
  virtual ~AddOperator();
};

class SubtractOperator : public BinaryOperator {
 public:
  SubtractOperator(Variable lhs, Variable rhs);
  static std::string keyword();
  size_t eval(const registers::Register &r) const override;
  virtual ~SubtractOperator();
};

class ComparisonOperator {
 protected:
  std::string keyword;
  Variable lhs;
  Variable rhs;

 public:
  ComparisonOperator(std::string keyword, Variable lhs, Variable rhs);
  std::string getKeyword() const;
  virtual bool eval(const registers::Register &r) const = 0;
  virtual ~ComparisonOperator();
};

class EqualityOperator : public ComparisonOperator {
 public:
  EqualityOperator(Variable lhs, Variable rhs);
  static std::string keyword();
  bool eval(const registers::Register &r) const override;
  virtual ~EqualityOperator();
};

class LessThanOperator : public ComparisonOperator {
 public:
  LessThanOperator(Variable lhs, Variable rhs);
  static std::string keyword();
  bool eval(const registers::Register &r) const override;
  virtual ~LessThanOperator();
};

class Operator {
 protected:
  std::string keyword;
 public:
  explicit Operator(std::string keyword);
  std::string getKeyword() const;
  virtual size_t exec(size_t programCounter, registers::Register &r) const = 0;
  virtual ~Operator();
};

class AssignmentOperator : public Operator {
 private:
  size_t regNumber;
  std::unique_ptr<BinaryOperator> op;
 public:
  AssignmentOperator(size_t regNumber, std::unique_ptr<BinaryOperator> op);
  static std::string keyword();
  size_t exec(size_t programCounter, registers::Register &r) const override;
  virtual ~AssignmentOperator();
};

class GotoOperator : public Operator {
 private:
  size_t lineNumber;
 public:
  explicit GotoOperator(size_t lineNumber);
  static std::string keyword();
  size_t exec(size_t programCounter, registers::Register &r) const override;
  virtual ~GotoOperator();
};

class ReturnOperator : public Operator {
 private:
  size_t returnRegNumber;
 public:
  explicit ReturnOperator(size_t returnRegNumber);
  static std::string keyword();
  size_t exec(size_t programCounter, registers::Register &r) const override;
  virtual ~ReturnOperator();
};

class IfOperator : public Operator {
 private:
  size_t lineNumber;
  std::unique_ptr<ComparisonOperator> op;
 public:
  IfOperator(size_t lineNumber, std::unique_ptr<ComparisonOperator> op);
  static std::string keyword();
  size_t exec(size_t programCounter, registers::Register &r) const override;
  virtual ~IfOperator();
};
}

#endif