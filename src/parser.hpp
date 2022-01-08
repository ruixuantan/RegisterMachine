#ifndef PARSER_H
#define PARSER_H

#include "operators.hpp"

#include <string>
#include <vector>
#include <exception>
#include <string_view>

namespace ParseLib {
  int getLineLength(std::string_view line, const int& lineNumber);
  int getRegisterNumber(const std::string& line, const int& lineNumber);
  int getNumber(const std::string& line, const int& lineNumber);
  std::vector<std::string> tokenise(std::string_view line, const int& length);
  Variable parseVariable(const std::string& token, const int& lineNumber);
  std::shared_ptr<ReturnOperator> parseReturn(const std::vector<std::string>& line, const int& lineNumber);
  std::shared_ptr<GotoOperator> parseGoto(const std::vector<std::string>& line, const int& lineNumber);
  std::shared_ptr<AssignmentOperator> parseAssignment(const std::vector<std::string>& tokens, const int& lineNumber);
  std::shared_ptr<IfOperator> parseIf(const std::vector<std::string>& tokens, const int& lineNumber);
}

class Parser {
  private:
    std::shared_ptr<Operator> parseLine(std::string_view line, const int& length, const int& lineNumber);
  public:
    std::vector<int> parseInitialArgs(int argc, char **argv);
    std::vector<int> parseDeclarationLine(std::string_view line);
    std::vector<std::shared_ptr<Operator>> parse(const std::vector<std::string>& lines);
};

class ParseException: public std::exception {
  private:
    std::string error;
    int lineNumber;
  public:
    ParseException(std::string_view error, int lineNumber);
    int getLineNumber() const;
    const char* what() const noexcept override;
};

class ParseArgsException: public std::exception {
  private:
    std::string error;
  public:
    explicit ParseArgsException(std::string_view error);
    const char* what() const noexcept override;
};

#endif