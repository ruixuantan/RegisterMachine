#include "gtest/gtest.h"
#include "operators.hpp"

TEST(Variable_test, equality) {
  EXPECT_TRUE (Variable(3, true) == Variable(3, true));
  EXPECT_FALSE (Variable(3, true) == Variable(3, false));
}

TEST(BinaryOperator_test, equality) {
  EXPECT_TRUE (AddOperator(Variable(3, true), Variable(4, false)) 
    == AddOperator(Variable(3, true), Variable(4, false)));
  EXPECT_FALSE (AddOperator(Variable(3, true), Variable(4, false)) 
    == AddOperator(Variable(3, true), Variable(4, true)));
}