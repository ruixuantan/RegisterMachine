#ifndef REGISTER_H
#define REGISTER_H

#include <array>
#include <string>

const int MAX_REG_SIZE { 100 };

class Register {
  private:
    std::array<int, MAX_REG_SIZE> reg;
  public:
    Register();
    void setRegister(int value, int regNumber);
    void setRegisters(std::vector<int> values, std::vector<int> regNumbers);
    int getRegister(int regNumber) const;

    std::string printRegister(int regNumber) const;
    std::string printRegisters(int start, int end) const;
};

#endif