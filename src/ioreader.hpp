#ifndef IOREADER_H
#define IOREADER_H

#include <string>

namespace IOReader {
  std::vector<std::string> readFile(std::string filename);
  std::vector<int> readInitialArgs(int argc, char **argv);
  void print(std::string toPrint);
};

#endif