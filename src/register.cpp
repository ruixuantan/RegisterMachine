#include "register.hpp"

#include <array>
#include <string>
#include <vector>
#include <string_view>

using namespace registers;

Register::Register() {
  for (size_t& i : this->reg) {
    i = 0;
  }
}

void Register::setRegister(const size_t& value, const size_t& regNumber) {
  if (value < 0) {
    throw RegisterException(regNumber, "Values must be less than 0"); 
  }
  reg[regNumber - 1] = value;
}

void Register::setRegisters(const std::vector<size_t>& values, const std::vector<size_t>& regNumbers) {
  for (size_t i {0}; i < values.size(); ++i) {
    if (values[i] < 0) {
      throw RegisterException(regNumbers[i], "Values must be less than 0"); 
    }
    reg[regNumbers[i] - 1] = values[i];
  }
}

size_t Register::getRegister(const size_t& regNumber) const {
  return reg[regNumber - 1];
}

std::string Register::printRegister(const size_t& regNumber) const {
  return "r" + std::to_string(regNumber) + ": " + std::to_string(reg[regNumber - 1]);
}

std::string Register::printRegisters(const size_t& start, const size_t& end) const {
  std::string registerString;
  for (size_t i {start - 1}; i < end; ++i) {
    registerString += "r" + std::to_string(i + 1) + ": " + std::to_string(reg[i]) +"\n";
  }
  return registerString;
}

RegisterException::RegisterException(size_t regNumber, std::string_view error)
  : regNumber{regNumber}, error{error} {}

size_t RegisterException::getRegNumber() const {
  return regNumber;
}

const char* RegisterException::what() const noexcept {
  return error.c_str();
}