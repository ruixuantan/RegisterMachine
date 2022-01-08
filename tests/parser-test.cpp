#include "gtest/gtest.h"
#include "parser.hpp"
#include "operators.hpp"
#include "register.hpp"

#include <vector>
#include <string>

TEST(ParserLib_test, tokenise) {
  const std::string ifStatement {"if r1 < r2 goto 3"};
  std::vector<std::string> ifExpected {"if", "r1", "<", "r2", "goto", "3"};
  EXPECT_TRUE(ifExpected == ParseLib::tokenise(ifStatement, 17));

  const std::string assignment {"r1 = 3 + r5"};
  std::vector<std::string> assignmentExpected {"r1", "=", "3", "+", "r5"};
  EXPECT_TRUE(assignmentExpected == ParseLib::tokenise(assignment, 11));
}

TEST(ParserLib_test, getLineLength) {
  int mockLineNumber { 0 };

  std::string normal { "string 1" };
  EXPECT_EQ (8, ParseLib::getLineLength(normal, mockLineNumber));
  
  std::string whitespace { "string 1 " };
  EXPECT_EQ (8, ParseLib::getLineLength(whitespace, mockLineNumber));
  
  std::string comment { "string # 111" };
  EXPECT_EQ (6, ParseLib::getLineLength(comment, mockLineNumber));
}

TEST(ParserLib_test, parseReturn) {
  int mockLineNumber { 0 };
  Register r {};
  r.setRegister(8, 8);
  r.setRegister(18, 18);

  std::vector<std::string> tokensSingle {"5.", "return", "r8"};
  EXPECT_EQ (8, ParseLib::parseReturn(tokensSingle, mockLineNumber)->exec(0, r));
  
  std::vector<std::string> tokensDouble {"5.", "return", "r18"};
  EXPECT_EQ (18, ParseLib::parseReturn(tokensDouble, mockLineNumber)->exec(0, r));
}

TEST(ParserLib_test, parseGoto) {
  int mockLineNumber { 0 };
  Register r {};

  std::vector<std::string> tokensSingle {"5.", "goto", "8"};
  EXPECT_EQ (8, ParseLib::parseGoto(tokensSingle, mockLineNumber)->exec(0, r));
}

TEST(ParserLib_test, parseAssignment) {
  int mockLineNumber { 0 };
  std::string addOperator {"+"};
  std:: string subtractOperator {"-"};
  Register r {};

  std::vector<std::string> addDoubleValueStmt {"2.", "r1", "=", "3", "+", "4"};
  std::shared_ptr<AssignmentOperator> addDoubleValue {
    ParseLib::parseAssignment(addDoubleValueStmt, mockLineNumber)
  };
  EXPECT_TRUE (2 == addDoubleValue->exec(1, r));
  EXPECT_EQ (r.getRegister(1), 7);

  std::vector<std::string> singleValueStmt {"3.", "r2", "=", "3"};
  ParseLib::parseAssignment(singleValueStmt, mockLineNumber)->exec(1, r);
  EXPECT_EQ (r.getRegister(2), 3);

  r.setRegister(13, 13);
  std::vector<std::string> subtractStmt {"4.", "r3", "=", "r13", "-", "3"};
  ParseLib::parseAssignment(subtractStmt, mockLineNumber)->exec(1, r);
  EXPECT_EQ (r.getRegister(3), 10);

  r.setRegister(14, 14);
  r.setRegister(15, 15);
  std::vector<std::string> addStmt {"5.", "r4", "=", "r14", "+", "r15"};
  ParseLib::parseAssignment(addStmt, mockLineNumber)->exec(1, r);
  EXPECT_EQ (r.getRegister(4), 29);
}

TEST(ParserLib_test, parseIf) {
  int mockLineNumber{0};
  Register r {};

  r.setRegister(2, 1);
  std::vector<std::string>  lessThanStmt {"5.", "if", "r1", "<", "3", "goto", "10"};
  std::shared_ptr<IfOperator> lessThanExpected {
    ParseLib::parseIf(lessThanStmt, mockLineNumber)
  };
  EXPECT_TRUE(IfOperator::keyword == lessThanExpected->getKeyword());
  EXPECT_EQ(9, lessThanExpected->exec(0, r));

  r.setRegister(10, 1);
  EXPECT_EQ(1, lessThanExpected->exec(0, r));

  r.setRegister(2, 1);
  std::vector<std::string>  equalStmt {"5.", "if", "r1", "==", "2", "goto", "10"};
  std::shared_ptr<IfOperator> equalExpected {
    ParseLib::parseIf(equalStmt, mockLineNumber)
  };
  EXPECT_TRUE(IfOperator::keyword == equalExpected->getKeyword());
  EXPECT_EQ(9, equalExpected->exec(0, r));

  r.setRegister(10, 1);
  EXPECT_EQ(1, equalExpected->exec(0, r));
}
