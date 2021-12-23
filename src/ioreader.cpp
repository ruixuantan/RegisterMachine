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

  std::vector<int> readInitialArgs(int argc, char **argv) {
    std::vector<int> arguments{};
    for (int i = 2; i < argc; i++) {
      arguments.push_back(atoi(argv[i]));
    }
    return arguments;
  }

  void print(std::string toPrint) {
    std::cout << toPrint << "\n";
  }
}