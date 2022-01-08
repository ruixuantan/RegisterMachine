#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "operators.hpp"
#include "register.hpp"

#include <exception>
#include <vector>
#include <string_view>

class Executor {
  public:
    int execute(const std::vector<std::shared_ptr<Operator>>& operators, Register& r);
};

class RuntimeException: public std::exception {
  private:
    std::string error;
  public:
    explicit RuntimeException(std::string_view error);
    const char* what() const noexcept override;
};

#endif