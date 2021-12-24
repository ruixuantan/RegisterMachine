#ifndef IOREADER_H
#define IOREADER_H

#include <string>

namespace IOReader {
  std::vector<std::string> readFile(std::string filename);
  void printSuccess(std::string registerToPrint);
  void printArgsError(std::string msg);
  void printCompilationError(std::string msg, int lineNumber);
  void printRuntimeError(std::string msg);
  void printRegisterError(std::string msg, int regNumber);
};

#endif