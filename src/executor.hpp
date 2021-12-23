#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "operators.hpp"
#include "register.hpp"

#include <exception>
#include <vector>

class Executor {
  public:
    int execute(std::vector<Operator*> operators, Register &r);
};

class RuntimeException: public std::exception {
  private:
    std::string error;
  public:
    RuntimeException(std::string error);
    const char* what() const noexcept override;
};

#endif