#include "gtest/gtest.h"
#include "parser.hpp"
#include "operators.hpp"

#include <string>

TEST(ParserLib_test, getLineLength) {
  int mockLineNumber { 0 };

  std::string normal { "string 1" };
  EXPECT_EQ (8, ParseLib::getLineLength(normal, mockLineNumber));
  
  std::string whitespace { "string 1 " };
  EXPECT_EQ (8, ParseLib::getLineLength(whitespace, mockLineNumber));
  
  std::string comment { "string # 111" };
  EXPECT_EQ (6, ParseLib::getLineLength(comment, mockLineNumber));
}

TEST(ParserLib_test, parseLineNumber) {
  int mockLineNumber { 0 };
  int idx { 0 };

  std::string statement {"5. r4 = 6"};
  EXPECT_EQ (3, ParseLib::parseLineNumber(statement, 9, idx, mockLineNumber));

  std::string statement2 {"15. r4 = 6"};
  EXPECT_EQ (4, ParseLib::parseLineNumber(statement2, 10, idx, mockLineNumber));
}

TEST(ParserLib_test, getToken) {
  int mockLineNumber { 0 };

  int assignmentIdx {3};
  std::string regToken {"r4"};
  std::string assignmentStatement {"5. r4 = 6"};
  EXPECT_TRUE (regToken == ParseLib::getToken(assignmentStatement, 9, assignmentIdx, mockLineNumber));

  int gotoIdx {3};
  std::string gotoToken {"goto"};
  std::string gotoStatement {"5. goto 8"};
  EXPECT_TRUE (gotoToken == ParseLib::getToken(gotoStatement, 9, gotoIdx, mockLineNumber));

  int returnIdx {3};
  std::string returnToken {"return"};
  std::string returnStatement {"5. return r11"};
  EXPECT_TRUE (returnToken == ParseLib::getToken(returnStatement, 13, returnIdx, mockLineNumber));
}

TEST(ParserLib_test, parseReturn) {
  int mockLineNumber { 0 };
  
  std::string singleDigit {"5. return r8"};
  EXPECT_EQ (8, ParseLib::parseReturn(singleDigit, 12, 10, mockLineNumber));
  
  std::string doubleDigit {"5. return r18"};
  EXPECT_EQ (18, ParseLib::parseReturn(doubleDigit, 13, 10, mockLineNumber));
}

TEST(ParserLib_test, parseAssignment) {
  int mockLineNumber { 0 };
  std::string addOperator {"+"};
  std:: string subtractOperator {"-"};

  std::string addDoubleValue {"r1 = 3 + 4"};
  std::unique_ptr<BinaryOperator> addDoubleActual {
    ParseLib::parseAssignment(addDoubleValue, 10, 3, mockLineNumber)
  };
  EXPECT_TRUE (addOperator == addDoubleActual->getKeyword());
  EXPECT_TRUE (Variable(3, false) == addDoubleActual->getLhs());
  EXPECT_TRUE (Variable(4, false) == addDoubleActual->getRhs());

  std::string singleValue {"r1 = 3"};
  std::unique_ptr<BinaryOperator> singleActual {
    ParseLib::parseAssignment(singleValue, 6, 3, mockLineNumber)
  };
  EXPECT_TRUE (Variable(3, false) == singleActual->getLhs());

  std::string subtractRegister {"r1 = r4 - 4"};
  std::unique_ptr<BinaryOperator> subtractRegisterActual {
    ParseLib::parseAssignment(subtractRegister, 11, 3, mockLineNumber)
  };
  EXPECT_TRUE (subtractOperator == subtractRegisterActual->getKeyword());
  EXPECT_TRUE (Variable(4, true) == subtractRegisterActual->getLhs());
  EXPECT_TRUE (Variable(4, false) == subtractRegisterActual->getRhs());

  std::string addDoubleRegister {"r1 = r4 + r11"};
  std::unique_ptr<BinaryOperator> addDoubleRegisterActual {
    ParseLib::parseAssignment(addDoubleRegister, 13, 3, mockLineNumber)
  };
  EXPECT_TRUE (addOperator == addDoubleRegisterActual->getKeyword());
  EXPECT_TRUE (Variable(4, true) == addDoubleRegisterActual->getLhs());
  EXPECT_TRUE (Variable(11, true) == addDoubleRegisterActual->getRhs());
}