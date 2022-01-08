#include "ioreader.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string_view>

namespace IOReader {
  const std::vector<std::string> readFile(const std::string& filename) {
    std::vector<std::string> vector {};
    std::fstream newfile;

    newfile.open(filename, std::ios::in);
    if (newfile.is_open()) {
      std::string line;
      while (getline(newfile, line)) {
        vector.push_back(line);
      }
    }
    newfile.close();

    return vector;
  };

  void printSuccess(const std::string_view registerToPrint) {
    std::cout << "Program terminated successfully\n" << registerToPrint << "\n";
  }

  void printArgsError(const std::string_view msg) {
    std::cerr << "Argument error: " << msg << "\n";
  }

  void printCompilationError(const std::string_view msg, const int& lineNumber) {
    std::cerr << "Compilation error on line: " << lineNumber << "\nError: " << msg << "\n";
  }
  
  void printRuntimeError(const std::string_view msg) {
    std::cerr << "Program terminated unsuccessfully:\n" << msg << "\n";
  }

  void printRegisterError(const std::string_view msg, const int& regNumber) {
    std::cerr << "Register error on r" << regNumber << ":\n" << msg << "\n";
  }
}