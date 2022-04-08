#ifndef IOREADER_H
#define IOREADER_H

#include <string>
#include <string_view>
#include <vector>

namespace ioreader {
std::vector<std::string> readFile(const std::string& filename);
void printSuccess(std::string_view registerToPrint);
void printArgsError(std::string_view msg);
void printCompilationError(std::string_view msg, const size_t& lineNumber);
void printRuntimeError(std::string_view msg);
void printRegisterError(std::string_view msg, const size_t& regNumber);
}

#endif