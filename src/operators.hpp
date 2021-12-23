#ifndef OPERATOR_H
#define OPERATOR_H

#include "register.hpp"

#include <string>

const int TERMINATE_LINE_NUMBER { -1 };

class BinaryOperator {
  protected:
    std::string keyword;
    int lhsToken;
    int rhsToken;

  public:
    BinaryOperator(std::string keyword, int lhsToken, int rhsToken);
    std::string getKeyword() const;
};

class AddOperator: public BinaryOperator {
  public:
    AddOperator(int lhsToken, int rhsToken);
    static std::string keyword;
    int eval() const;
};

class SubtractOperator: public BinaryOperator {
  public:
    SubtractOperator(int lhsToken, int rhsToken);
    static std::string keyword;
    int eval() const;
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
    virtual int exec(int programCounter, Register &r) const;
};

class AssignmentOperator: public Operator {
  private:
    int regNumber;
    int value;
  public:
    AssignmentOperator(int regNumber, int value);
    static std::string keyword;
    int exec(int programCounter, Register &r) const override;
};

class ReturnOperator: public Operator {
  private:
    int returnRegNumber;
  public:
    ReturnOperator(int returnRegNumber);
    static std::string keyword;
    int exec(int programCounter, Register &r) const override;
};

#endif