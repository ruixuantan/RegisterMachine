#include "ioreader.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace IOReader {
  std::vector<std::string> readFile(std::string filename) {
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

  void printSuccess(std::string registerToPrint) {
    std::cout << "Program terminated successfully\n" << registerToPrint << "\n";
  }

  void printArgsError(std::string msg) {
    std::cerr << "Argument error: " << msg << "\n";
  }

  void printCompilationError(std::string msg, int lineNumber) {
    std::cerr << "Compilation error on line: " << lineNumber << "\nError: " << msg << "\n";
  }
  
  void printRuntimeError(std::string msg) {
    std::cerr << "Program terminated unsuccessfully:\n" << msg << "\n";
  }
}