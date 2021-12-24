#ifndef REGISTER_H
#define REGISTER_H

#include <array>
#include <string>
#include <exception>

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

class RegisterException: public std::exception {
  private:
    int regNumber;
    std::string error;
  public:
    RegisterException(int regNumber, std::string error);
    int getRegNumber() const;
    const char* what() const noexcept override;
};

#endif