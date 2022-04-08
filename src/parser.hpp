#ifndef PARSER_H
#define PARSER_H

#include "operators.hpp"

#include <string>
#include <vector>
#include <exception>
#include <string_view>

namespace parser {
constexpr char BeginBracketToken = '(';
constexpr char EndBracketToken = ')';
constexpr char RegisterDelimiter = ',';
constexpr char RegisterToken = 'r';
constexpr char DelimiterToken = ' ';
constexpr char CommentToken = '#';

size_t getLineLength(std::string_view line, const size_t &lineNumber);
size_t getRegisterNumber(const std::string &line, const size_t &lineNumber);
size_t getNumber(const std::string &line, const size_t &lineNumber);
std::vector<std::string> tokenise(std::string_view line, const size_t &length);
operators::Variable parseVariable(const std::string &token, const size_t &lineNumber);
std::shared_ptr<operators::ReturnOperator> parseReturn(const std::vector<std::string> &line, const size_t &lineNumber);
std::shared_ptr<operators::GotoOperator> parseGoto(const std::vector<std::string> &line, const size_t &lineNumber);
std::shared_ptr<operators::AssignmentOperator> parseAssignment(const std::vector<std::string> &tokens, const size_t &lineNumber);
std::shared_ptr<operators::IfOperator> parseIf(const std::vector<std::string> &tokens, const size_t &lineNumber);

class Parser {
 private:
  static std::shared_ptr<operators::Operator> parseLine(std::string_view line,
                                                        const size_t &length,
                                                        const size_t &lineNumber);
 public:
  static std::vector<size_t> parseInitialArgs(int argc, char **argv);
  static std::vector<size_t> parseDeclarationLine(std::string_view line);
  static std::vector<std::shared_ptr<operators::Operator>> parse(const std::vector<std::string> &lines);
};

class ParseException : public std::exception {
 private:
  std::string error;
  size_t lineNumber;
 public:
  ParseException(std::string_view error, size_t lineNumber);
  size_t getLineNumber() const;
  const char *what() const noexcept override;
};

class ParseArgsException : public std::exception {
 private:
  std::string error;
 public:
  explicit ParseArgsException(std::string_view error);
  const char *what() const noexcept override;
};
}

#endif