#ifndef OPERATOR_H
#define OPERATOR_H

#include "register.hpp"

#include <string>

class Variable {
  private:
    int value;
    bool isRegister;
  public:
    Variable(int value=0, bool isRegister=false);
    const int eval(const Register& r) const;
    friend bool operator== (const Variable& v1, const Variable& v2);
};

class BinaryOperator {
  protected:
    std::string keyword;
    Variable lhs;
    Variable rhs;

  public:
    BinaryOperator(std::string keyword, Variable lhs, Variable rhs);
    const std::string getKeyword() const;
    const Variable getLhs() const;
    const Variable getRhs() const;
    friend bool operator== (const BinaryOperator& o1, const BinaryOperator& o2);
    virtual const int eval(const Register& r) const = 0;
    virtual ~BinaryOperator();
};

class AddOperator: public BinaryOperator {
  public:
    AddOperator(Variable lhs, Variable rhs);
    const static std::string keyword;
    const int eval(const Register& r) const override;
    virtual ~AddOperator();
};

class SubtractOperator: public BinaryOperator {
  public:
    SubtractOperator(Variable lhs, Variable rhs);
    const static std::string keyword;
    const int eval(const Register& r) const override;
    virtual ~SubtractOperator();
};

class ComparisonOperator {
   protected:
    std::string keyword;
    int lhsToken;
    int rhsToken;

  public:
    ComparisonOperator(std::string keyword, int lhsToken, int rhsToken);
    std::string getKeyword() const;
};

class EqualityOperator: public ComparisonOperator {
  public:
    EqualityOperator(int lhsToken, int rhsToken);
    static std::string keyword;
    bool eval() const; 
};

class LessThanOperator: public ComparisonOperator {
  public:
    LessThanOperator(int lhsToken, int rhsToken);
    static std::string keyword;
    bool eval() const; 
};

class Operator {
  protected:
    std::string keyword;
  public:
    Operator(std::string keyword);
    const std::string getKeyword() const;
    virtual const int exec(int programCounter, Register& r) const = 0;
    virtual ~Operator();
};

class AssignmentOperator: public Operator {
  private:
    int regNumber;
    std::unique_ptr<BinaryOperator> op;
  public:
    AssignmentOperator(int regNumber, std::unique_ptr<BinaryOperator> op);
    const static std::string keyword;
    const int exec(int programCounter, Register& r) const override;
    virtual ~AssignmentOperator();
};

class GotoOperator: public Operator {
  private:
    int lineNumber;
  public:
    GotoOperator(int lineNumber);
    const static std::string keyword;
    const int exec(int programCounter, Register& r) const override;
    virtual ~GotoOperator();

};

class ReturnOperator: public Operator {
  private:
    int returnRegNumber;
  public:
    ReturnOperator(int returnRegNumber);
    const static std::string keyword;
    const int exec(int programCounter, Register& r) const override;
    virtual ~ReturnOperator();
};

#endif