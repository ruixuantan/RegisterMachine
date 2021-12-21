#include "ioreader.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::vector<std::string> IOReader::readFile(std::string filename) {
  std::vector<std::string> vector;
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