#include "operators.hpp"
#include "executor.hpp"
#include "register.hpp"

#include <vector>
#include <string_view>

int Executor::execute(const std::vector<std::shared_ptr<Operator>>& operators, Register& r) {
  int programCounter {0};

  while(operators[programCounter]->getKeyword() != ReturnOperator::keyword) {
    const std::shared_ptr<Operator> op { operators[programCounter] };
    std::string currKeyword { op->getKeyword() };
  
    if (currKeyword == AssignmentOperator::keyword) {
      programCounter = op->exec(programCounter, r);
    } else if (currKeyword == GotoOperator::keyword) {
      programCounter = op->exec(programCounter, r) - 1;
    } else if (currKeyword == IfOperator::keyword) {
      programCounter = op->exec(programCounter, r);
    } else {
      throw RuntimeException("An unknown error occurred");
    }

    if (programCounter >= operators.size()) {
      throw RuntimeException("Line number passed to goto command exceeded program lines");
    }
  }

  int returnRegister { operators[programCounter]->exec(programCounter, r) };
  return returnRegister;
}

RuntimeException::RuntimeException(std::string_view error)
  : error{error} {}

const char* RuntimeException::what() const noexcept {
  return this->error.c_str();
}
