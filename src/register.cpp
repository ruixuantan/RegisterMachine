#include "register.hpp"
#include "operators.hpp"

#include <array>
#include <string>
#include <vector>

Register::Register() {
  for (int& i : this->reg) {
    i = 0;
  }
}

void Register::setRegister(int value, int regNumber) {
  if (value < 0) {
    throw RegisterException(regNumber, "Values must be less than 0"); 
  }
  this->reg[regNumber - 1] = value;
}

void Register::setRegisters(std::vector<int> values, std::vector<int> regNumbers) {
  for (int i = 0; i < values.size(); i++) {
    if (values[i] < 0) {
      throw RegisterException(regNumbers[i], "Values must be less than 0"); 
    }
    this->reg[regNumbers[i] - 1] = values[i];
  }
}

int Register::getRegister(int regNumber) const {
  return this->reg[regNumber - 1];
}

std::string Register::printRegister(int regNumber) const {
  return "r" + std::to_string(regNumber) + ": " + std::to_string(this->reg[regNumber - 1]);
}

std::string Register::printRegisters(int start, int end) const {
  std::string registerString = "";
  for (int i = start - 1; i < end; i++) {
    registerString += "r" + std::to_string(i + 1) + ": " + std::to_string(this->reg[i]) +"\n";
  }
  return registerString;
}

RegisterException::RegisterException(int regNumber, std::string error)
  : regNumber{regNumber}, error{error} {}

int RegisterException::getRegNumber() const {
  return this->regNumber;
}

const char* RegisterException::what() const noexcept {
  return this->error.c_str();
}