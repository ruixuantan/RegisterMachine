#ifndef REGISTER_H
#define REGISTER_H

#include <array>
#include <string>
#include <exception>
#include <string_view>
#include <vector>

constexpr int MAX_REG_SIZE { 100 };

class Register {
  private:
    std::array<int, MAX_REG_SIZE> reg{};
  public:
    Register();
    void setRegister(const int& value, const int& regNumber);
    void setRegisters(const std::vector<int>& values, const std::vector<int>& regNumbers);
    const int getRegister(const int& regNumber) const;

    const std::string printRegister(const int& regNumber) const;
    const std::string printRegisters(const int& start, const int& end) const;
};

class RegisterException: public std::exception {
  private:
    int regNumber;
    std::string error;
  public:
    RegisterException(int regNumber, std::string_view error);
    const int getRegNumber() const;
    const char* what() const noexcept override;
};

#endif