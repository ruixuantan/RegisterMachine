#include "operators.hpp"

#include <string>
#include <vector>
#include <exception>

namespace ParseLib {
  int getLineLength(std::string &line, int lineNumber);
  int getRegisterNumber(std::string &registerStr, int lineNumber);
  int getNumber(std::string &numberStr, int lineNumber);
  int parseNumberToEnd(std::string &line, int length, int idx, int lineNumber);
  int parseLineNumber(std::string &line, int length, int idx, int lineNumber);
  std::string getToken(std::string &line, int length, int &idx, int lineNumber);
  int parseReturn(std::string &line, int length, int idx, int lineNumber);
  std::unique_ptr<BinaryOperator> parseAssignment(std::string &line, int length, int idx, int lineNumber);
}

class Parser {
  private:
    Operator* parseLine(std::string &line, int length, int lineNumber);
  public:
    std::vector<int> parseInitialArgs(int argc, char **argv);
    std::vector<int> parseDeclarationLine(std::string &line);
    std::vector<Operator*> parse(std::vector<std::string> lines); 
};

class ParseException: public std::exception {
  private:
    std::string error;
    int lineNumber;
  public:
    ParseException(std::string error, int lineNumber);
    int getLineNumber() const;
    const char* what() const noexcept override;
};

class ParseArgsException: public std::exception {
  private:
    std::string error;
  public:
    ParseArgsException(std::string error);
    const char* what() const noexcept override;
};