#ifndef OPERATOR_H
#define OPERATOR_H

#include "register.hpp"

#include <string>

const int TERMINATE_LINE_NUMBER { -1 };

class Variable {
  private:
    int value;
    bool isRegister;
  public:
    Variable();
    Variable(int value, bool isRegister);
    int eval(Register &r) const;
    friend bool operator== (const Variable& v1, const Variable& v2);
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
    friend bool operator== (const BinaryOperator& o1, const BinaryOperator& o2);
    virtual int eval(Register &r) const = 0;
    virtual ~BinaryOperator();
};

class AddOperator: public BinaryOperator {
  public:
    AddOperator(Variable lhs, Variable rhs);
    static std::string keyword;
    int eval(Register &r) const override;
    virtual ~AddOperator();
};

class SubtractOperator: public BinaryOperator {
  public:
    SubtractOperator(Variable lhs, Variable rhs);
    static std::string keyword;
    int eval(Register &r) const override;
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
    std::string getKeyword() const;
    virtual int exec(int programCounter, Register &r) const = 0;
    virtual ~Operator();
};

class AssignmentOperator: public Operator {
  private:
    int regNumber;
    std::unique_ptr<BinaryOperator> op;
  public:
    AssignmentOperator(int regNumber, std::unique_ptr<BinaryOperator> op);
    static std::string keyword;
    int exec(int programCounter, Register &r) const override;
    virtual ~AssignmentOperator();
};

class GotoOperator: public Operator {
  private:
    int lineNumber;
  public:
    GotoOperator(int lineNumber);
    static std::string keyword;
    int exec(int programCounter, Register &r) const override;
    virtual ~GotoOperator();

};

class ReturnOperator: public Operator {
  private:
    int returnRegNumber;
  public:
    ReturnOperator(int returnRegNumber);
    static std::string keyword;
    int exec(int programCounter, Register &r) const override;
    virtual ~ReturnOperator();
};

#endif