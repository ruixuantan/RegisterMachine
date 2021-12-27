#include "operators.hpp"

#include <string>
#include <vector>
#include <exception>
#include <string_view>

namespace ParseLib {
  const int getLineLength(const std::string_view line, const int& lineNumber);
  const int getRegisterNumber(const std::string& line, const int& lineNumber);
  const int getNumber(const std::string& line, const int& lineNumber);
  const int parseNumberToEnd(const std::string_view line, const int& length, int idx, const int& lineNumber);
  const int parseLineNumber(const std::string_view line, const int& length, int idx, const int& lineNumber);
  const std::string getToken(const std::string_view line, const int& length, int& idx, const int& lineNumber);
  const Variable parseVariable(const std::string token, const int& lineNumber);
  const int parseReturn(const std::string_view line, const int& length, int idx, const int& lineNumber);
  const std::unique_ptr<BinaryOperator> parseAssignment(const std::string_view line, const int& length, int idx, const int& lineNumber);
  const std::vector<std::string> tokenise(const std::string_view line, const int& length, int idx);
  IfOperator* parseIf(const std::string_view line, const int& length, int idx, const int& lineNumber);
}

class Parser {
  private:
    Operator* parseLine(const std::string_view line, const int& length, const int& lineNumber);
  public:
    const std::vector<int> parseInitialArgs(int argc, char **argv);
    const std::vector<int> parseDeclarationLine(std::string_view line);
    const std::vector<Operator*> parse(const std::vector<std::string> lines); 
};

class ParseException: public std::exception {
  private:
    std::string error;
    int lineNumber;
  public:
    ParseException(std::string_view error, int lineNumber);
    const int getLineNumber() const;
    const char* what() const noexcept override;
};

class ParseArgsException: public std::exception {
  private:
    std::string error;
  public:
    ParseArgsException(std::string_view error);
    const char* what() const noexcept override;
};