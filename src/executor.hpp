#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "operators.hpp"
#include "register.hpp"

#include <exception>
#include <vector>
#include <string_view>

class Executor {
  public:
    const int execute(const std::vector<Operator*> operators, Register& r);
    void cleanup(const std::vector<Operator*> operators);
};

class RuntimeException: public std::exception {
  private:
    std::string error;
  public:
    RuntimeException(std::string_view error);
    const char* what() const noexcept override;
};

#endif