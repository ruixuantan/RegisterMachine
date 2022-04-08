#ifndef REGISTER_H
#define REGISTER_H

#include <array>
#include <string>
#include <exception>
#include <string_view>
#include <vector>

namespace registers {
constexpr size_t MAX_REG_SIZE{1000};

class Register {
 private:
  std::array<size_t, MAX_REG_SIZE> reg{};
 public:
  Register();
  void setRegister(const size_t &value, const size_t &regNumber);
  void setRegisters(const std::vector<size_t> &values, const std::vector<size_t> &regNumbers);
  size_t getRegister(const size_t &regNumber) const;

  std::string printRegister(const size_t &regNumber) const;
  std::string printRegisters(const size_t &start, const size_t &end) const;
};

class RegisterException : public std::exception {
 private:
  size_t regNumber;
  std::string error;
 public:
  RegisterException(size_t regNumber, std::string_view error);
  size_t getRegNumber() const;
  const char *what() const noexcept override;
};
}

#endif