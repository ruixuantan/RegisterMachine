#ifndef IOREADER_H
#define IOREADER_H

#include <string>

class IOReader {
  public:
    std::vector<std::string> readFile(std::string filename);
};

#endif