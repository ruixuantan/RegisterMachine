#include "operators.hpp"
#include "executor.hpp"
#include "register.hpp"

#include <vector>
#include <iostream>

int Executor::execute(std::vector<Operator*> operators, Register &r) {
  int programCounter {0};

  while(operators[programCounter]->getKeyword() != ReturnOperator::keyword) {
    const Operator* op { operators[programCounter] };
    if (op->getKeyword() == AssignmentOperator::keyword) {
      programCounter = op->exec(programCounter, r);
    } else {
      return TERMINATE_LINE_NUMBER;
    }
  }

  int returnRegister { operators[programCounter]->exec(programCounter, r) };
  return returnRegister;
}
