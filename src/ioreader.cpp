#include "ioreader.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string_view>

using namespace ioreader;

std::vector<std::string> ioreader::readFile(const std::string& filename) {
  std::vector<std::string> vector {};
  std::fstream newfile (filename, std::ios::in);
  if (newfile.is_open()) {
    std::string line;
    while (getline(newfile, line)) {
      vector.push_back(line);
    }
  }
  newfile.close();

  return vector;
}

void ioreader::printSuccess(const std::string_view registerToPrint) {
  std::cout << "Program terminated successfully\n" << registerToPrint << "\n";
}

void ioreader::printArgsError(const std::string_view msg) {
  std::cerr << "Argument error: " << msg << "\n";
}

void ioreader::printCompilationError(const std::string_view msg, const int& lineNumber) {
  std::cerr << "Compilation error on line: " << lineNumber << "\nError: " << msg << "\n";
}

void ioreader::printRuntimeError(const std::string_view msg) {
  std::cerr << "Program terminated unsuccessfully:\n" << msg << "\n";
}

void ioreader::printRegisterError(const std::string_view msg, const int& regNumber) {
  std::cerr << "Register error on r" << regNumber << ":\n" << msg << "\n";
}