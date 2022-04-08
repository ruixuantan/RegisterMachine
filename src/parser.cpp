#include "parser.hpp"
#include "operators.hpp"

#include <memory>
#include <vector>
#include <string>
#include <string_view>

using namespace parser;
using namespace operators;

std::vector<int> Parser::parseInitialArgs(int argc, char **argv) {
  std::vector<int> arguments{};
  for (int i = 2; i < argc; i++) {
    try {
      arguments.push_back(std::stoi(argv[i]));
    } catch (const std::invalid_argument&) {
      throw ParseArgsException("Arguments passed to program must be an integer");
    }
  }
  return arguments;
}

/**
 * @brief Get the length of the effective line to be parsed.
 * The effective length excludes trailing whitespaces or comments.
 *
 * @param line
 * @param lineNumber current line number being parsed
 * @return int
 */
int parser::getLineLength(const std::string_view line, const int& lineNumber) {
  int length = line.size();
  int idx {length};
  for (int i = 0; i < length; i++) {
    if (line[i] == CommentToken) {
      idx = i;
      break;
    }
  }

  while(idx > 0) {
    if (line[idx - 1] == DelimiterToken) {
      idx--;
    } else {
      return idx;
    }
  }
  throw ParseException("Empty line detected", lineNumber);
}

/**
 * @brief Get the register number from a string.
 *
 * @param registerStr the register string, for example 'r4'/'r24'
 * @param lineNumber current line number being parsed
 * @return int
 */
int parser::getRegisterNumber(const std::string& registerStr, const int& lineNumber) {
  try {
    return std::stoi(registerStr.substr(1, registerStr.size()));
  } catch (const std::invalid_argument&) {
    throw ParseException("Register number passed is invalid", lineNumber);
  }
}

/**
 * @brief Converts number string to int
 *
 * @param numberStr
 * @param lineNumber
 * @return int
 */
int parser::getNumber(const std::string& numberStr, const int& lineNumber) {
  try {
    return std::stoi(numberStr);
  } catch (const std::invalid_argument&) {
    throw ParseException("Number passed is not an integer", lineNumber);
  }
}

std::vector<std::string> parser::tokenise(const std::string_view line, const int& length) {
  int idx{0};
  std::vector<std::string> tokens;
  std::string curr;
  while(idx < length) {
    if (line[idx] == DelimiterToken) {
      if (!curr.empty()) {
        tokens.push_back(curr);
        curr = "";
      }
    } else {
      curr += line[idx];
    }
    idx++;
  }
  if (!curr.empty()) {
    tokens.push_back(curr);
  }
  return tokens;
}

// Handle [X., return, rX]
std::shared_ptr<ReturnOperator> parser::parseReturn(const std::vector<std::string>& line, const int& lineNumber) {
  return std::make_shared<ReturnOperator> ( getRegisterNumber(line[2], lineNumber) );
}

// Handle [X., goto, X]
std::shared_ptr<GotoOperator> parser::parseGoto(const std::vector<std::string>& line, const int& lineNumber) {
  return std::make_shared<GotoOperator> ( getNumber(line[2], lineNumber) );
}

Variable parser::parseVariable(const std::string& token, const int& lineNumber) {
  Variable var{};
  if (token.front() == RegisterToken) {
    var = Variable(getRegisterNumber(token, lineNumber), true);
  } else {
    var = Variable(getNumber(token, lineNumber), false);
  }
  return var;
}

// Expecting [X., rX, =, X/rX, +, X/rX] OR [X., rX, =, X]
std::shared_ptr<AssignmentOperator> parser::parseAssignment(const std::vector<std::string>& tokens, const int& lineNumber) {
  const int regNumber {getRegisterNumber(tokens[1], lineNumber)};
  Variable lhsVar {parseVariable(tokens[3], lineNumber)};

  if (tokens.size() <= 4) {
    return std::make_shared<AssignmentOperator> (
        regNumber, std::make_unique<AddOperator>(lhsVar, Variable{})
    );
  }

  Variable rhsVar {parseVariable(tokens[5], lineNumber)};
  if (tokens[4] == AddOperator::keyword) {
    return std::make_shared<AssignmentOperator> (
        regNumber, std::make_unique<AddOperator>(lhsVar, rhsVar)
    );
  } else if (tokens[4] == SubtractOperator::keyword) {
    return std::make_shared<AssignmentOperator> (
        regNumber, std::make_unique<SubtractOperator>(lhsVar, rhsVar)
    );
  } else {
    throw ParseException("Operator is not recognised", lineNumber);
  }
}

// Handle [X. if, rX/X, </==, rX/X, goto, X]
std::shared_ptr<IfOperator> parser::parseIf(const std::vector<std::string>& tokens, const int& lineNumber) {
  Variable lhs {parseVariable(tokens[2], lineNumber)};
  Variable rhs {parseVariable(tokens[4], lineNumber)};

  if (tokens[5] != GotoOperator::keyword) {
    throw ParseException("Goto operator expected", lineNumber);
  }

  int returnLine {getNumber(tokens[6], lineNumber)};

  if (tokens[3] == LessThanOperator::keyword) {
    return std::make_shared<IfOperator> (
        returnLine, std::make_unique<LessThanOperator>(lhs, rhs)
    );
  } else if (tokens[3] == EqualityOperator::keyword) {
    return std::make_shared<IfOperator> (
        returnLine, std::make_unique<EqualityOperator>(lhs, rhs)
    );
  } else {
    throw ParseException("Comparison operator expected", lineNumber);
  }
}

std::shared_ptr<Operator> Parser::parseLine(const std::string_view line, const int& length, const int& lineNumber) {
  std::vector<std::string> tokens {tokenise(line, length)};

  // Start token could be 'return' / 'rX' / 'goto' / 'if'
  std::string startToken { tokens[1] };

  if (startToken == ReturnOperator::keyword) {
    return parseReturn(tokens, lineNumber);
  } else if (startToken == GotoOperator::keyword) {
    return parseGoto(tokens, lineNumber);
  } else if (startToken == IfOperator::keyword) {
    return parseIf(tokens, lineNumber);
  }

  if (startToken.front() == RegisterToken) {
    return parseAssignment(tokens, lineNumber);
  }

  throw ParseException("No valid syntax detected", lineNumber);
}

std::vector<int> Parser::parseDeclarationLine(std::string_view line) {
  int length ( line.length() );
  std::vector<int> registerNumbers {};

  int startIdx {0};
  int endIdx {0};
  for (int i = 0; i < length; i++) {
    if (line[i] == BeginBracketToken) {
      startIdx = i;
    }
    if (line[i] == EndBracketToken) {
      endIdx = i;
    }
  }

  if (startIdx == 0) {
    throw ParseException("Bracket '(' not found in declaration", 0);
  }

  if (endIdx == 0) {
    throw ParseException("Bracket ')' not found in declaration", 0);
  }

  int idx { startIdx };

  while(idx < endIdx) {
    if (line[idx] == RegisterToken) {
      std::string registerNumberStr;
      while(line[idx] != RegisterDelimiter && line[idx] != EndBracketToken) {
        registerNumberStr += line[idx];
        idx++;
      }
      int n = getRegisterNumber(registerNumberStr, 0);
      registerNumbers.push_back(n);
    }
    idx++;
  }
  return registerNumbers;
}

std::vector<std::shared_ptr<Operator>> Parser::parse(const std::vector<std::string>& lines) {
  std::vector<std::shared_ptr<Operator>> operators {};
  for (int i = 1; i < lines.size(); i++) {
    int lineLength { getLineLength(lines[i], i) };
    std::shared_ptr<Operator> o { parseLine(lines[i], lineLength, i) };
    operators.push_back(o);
  }
  return operators;
}

ParseException::ParseException(std::string_view error, int lineNumber)
    : error{error}, lineNumber{lineNumber} {}

int ParseException::getLineNumber() const {
  return this->lineNumber;
}

const char* ParseException::what() const noexcept {
  return this->error.c_str();
}

ParseArgsException::ParseArgsException(std::string_view error)
    : error{error} {}

const char* ParseArgsException::what() const noexcept {
  return this->error.c_str();
}