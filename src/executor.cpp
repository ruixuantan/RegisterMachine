#include "operators.hpp"
#include "executor.hpp"
#include "register.hpp"

#include <vector>
#include <iostream>

int Executor::execute(std::vector<Operator*> operators, Register &r) {
  int programCounter {0};

  while(operators[programCounter]->getKeyword() != ReturnOperator::keyword) {
    const Operator* op { operators[programCounter] };
    std::string currKeyword = op->getKeyword();
  
    if (currKeyword == AssignmentOperator::keyword) {
      programCounter = op->exec(programCounter, r);
    } else if (currKeyword == GotoOperator::keyword) {
      programCounter = op->exec(programCounter, r) - 1;
    } else {
      return TERMINATE_LINE_NUMBER;
    }

    if (programCounter >= operators.size()) {
      throw RuntimeException("Line number passed to goto command exceeded program lines");
    }
  }

  int returnRegister { operators[programCounter]->exec(programCounter, r) };
  return returnRegister;
}

RuntimeException::RuntimeException(std::string error)
  : error{error} {}

const char* RuntimeException::what() const noexcept {
  return this->error.c_str();
}
