#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "operators.hpp"
#include "register.hpp"

#include <vector>

class Executor {
  public:
    int execute(std::vector<Operator*> operators, Register &r);
};

#endif