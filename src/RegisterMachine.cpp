#include "ioreader.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main(int argc, char **argv) {
  std::string filename = argv[1];

  IOReader io;
  std::vector<std::string> vector = io.readFile(filename);
  for (int i = 0; i < vector.size(); i++) {
    std::cout << vector[i] << "\n";
  }

  return 0;
}
