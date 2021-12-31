#ifndef IOREADER_H
#define IOREADER_H

#include <string>
#include <string_view>
#include <vector>

namespace IOReader {
  const std::vector<std::string> readFile(const std::string& filename);
  void printSuccess(const std::string_view registerToPrint);
  void printArgsError(const std::string_view msg);
  void printCompilationError(const std::string_view msg, const int& lineNumber);
  void printRuntimeError(const std::string_view msg);
  void printRegisterError(const std::string_view msg, const int& regNumber);
};

#endif