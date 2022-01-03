#include "parser.hpp"
#include "operators.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include <string_view>
#include <memory>

constexpr char BeginBracketToken = '(';
constexpr char EndBracketToken = ')';
constexpr char RegisterDelimiter = ',';
constexpr char LineNumberToken = '.';
constexpr char RegisterToken = 'r';
constexpr char DelimiterToken = ' ';
constexpr char CommentToken = '#';

const std::vector<int> Parser::parseInitialArgs(int argc, char **argv) {
  std::vector<int> arguments{};
  for (int i = 2; i < argc; i++) {
    try {
      arguments.push_back(std::stoi(argv[i]));
    } catch (std::invalid_argument) {
      throw ParseArgsException("Arguments passed to program must be an integer");
    }
  }
  return arguments;
}

namespace ParseLib {
  /**
   * @brief Get the length of the effective line to be parsed.
   * The effective length excludes trailing whitespaces or comments.
   * 
   * @param line 
   * @param lineNumber current line number being parsed
   * @return int 
   */
  const int getLineLength(const std::string_view line, const int& lineNumber) {
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
  inline const int getRegisterNumber(const std::string& registerStr, const int& lineNumber) {
    try {
      return std::stoi(registerStr.substr(1, registerStr.size()));
    } catch (std::invalid_argument) {
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
  inline const int getNumber(const std::string& numberStr, const int& lineNumber) {
    try {
      return std::stoi(numberStr);
    } catch (std::invalid_argument) {
      throw ParseException("Number passed is not an integer", lineNumber);
    }
  }

  /**
   * Expects a '.' to terminate the line.
   */
  inline const int getLineNumber(const std::string& line, const int& lineNumber) {
    if (line.back() != LineNumberToken) {
      throw ParseException("Line number did not terminate with '.'", lineNumber);
    }
    return getNumber(line.substr(0, line.size() - 1), lineNumber);
  }

  const std::vector<std::string> tokenise(const std::string_view line, const int& length) {
    int idx{0};
    std::vector<std::string> tokens;
    std::string curr {""};
    while(idx < length) {
      if (line[idx] == DelimiterToken) {
        if (curr != "") {
          tokens.push_back(curr);
          curr = "";
        }
      } else {
        curr += line[idx];
      }
      idx++;
    }
    if (curr != "") {
      tokens.push_back(curr);
    }
    return tokens;
  }

  // Handle [X., return, rX]
  std::shared_ptr<ReturnOperator> parseReturn(const std::vector<std::string> line, const int& lineNumber) {
    return std::shared_ptr<ReturnOperator> { new ReturnOperator{getRegisterNumber(line[2], lineNumber)} };
  }
  
  // Handle [X., goto, X]
  std::shared_ptr<GotoOperator> parseGoto(const std::vector<std::string> line, const int& lineNumber) {
    return std::shared_ptr<GotoOperator> { new GotoOperator{getNumber(line[2], lineNumber)} };
  }

  const Variable parseVariable(const std::string token, const int& lineNumber) {
    Variable var{};
    if (token.front() == RegisterToken) {
      var = Variable(getRegisterNumber(token, lineNumber), true);
    } else {
      var = Variable(getNumber(token, lineNumber), false);
    }
    return var;
  }
  
  // Expecting [X., rX, =, X/rX, +, X/rX] OR [X., rX, =, X]
  std::shared_ptr<AssignmentOperator> parseAssignment(const std::vector<std::string> tokens, const int& lineNumber) {
    const int regNumber {getRegisterNumber(tokens[1], lineNumber)};
    Variable lhsVar {parseVariable(tokens[3], lineNumber)};

    if (tokens.size() <= 4) {
      return std::shared_ptr<AssignmentOperator> { 
        new AssignmentOperator(regNumber, std::unique_ptr<AddOperator>(new AddOperator(lhsVar, Variable{}))) 
      };
    }

    Variable rhsVar {parseVariable(tokens[5], lineNumber)};
    if (tokens[4] == AddOperator::keyword) {
      return std::shared_ptr<AssignmentOperator> {
        new AssignmentOperator(regNumber, std::unique_ptr<AddOperator>(new AddOperator(lhsVar, rhsVar)))
      };
    } else if (tokens[4] == SubtractOperator::keyword) {
      return std::shared_ptr<AssignmentOperator> {
        new AssignmentOperator(regNumber, std::unique_ptr<SubtractOperator>(new SubtractOperator(lhsVar, rhsVar)))
      };
    } else {
      throw ParseException("Operator is not recognised", lineNumber);
    } 
  }

  // Handle [X. if, rX/X, </==, rX/X, goto, X]
  std::shared_ptr<IfOperator> parseIf(const std::vector<std::string> tokens, const int& lineNumber) {
    Variable lhs {parseVariable(tokens[2], lineNumber)};
    Variable rhs {parseVariable(tokens[4], lineNumber)};

    if (tokens[5] != GotoOperator::keyword) {
      throw ParseException("Goto operator expected", lineNumber);
    }

    int returnLine {getNumber(tokens[6], lineNumber)};
    
    if (tokens[3] == LessThanOperator::keyword) {
      return std::shared_ptr<IfOperator> {
        new IfOperator(returnLine, std::unique_ptr<LessThanOperator>(new LessThanOperator(lhs, rhs)))
      };
    } else if (tokens[3] == EqualityOperator::keyword) {
      return std::shared_ptr<IfOperator> {
        new IfOperator(returnLine, std::unique_ptr<EqualityOperator>(new EqualityOperator(lhs, rhs)))
      };
    } else {
      throw ParseException("Comparison operator expected", lineNumber);
    }
  }
}

std::shared_ptr<Operator> Parser::parseLine(const std::string_view line, const int& length, const int& lineNumber) {
  std::vector<std::string> tokens {ParseLib::tokenise(line, length)};

  int currentLineNumber { ParseLib::getLineNumber(tokens[0], lineNumber) };
  // Start token could be 'return' / 'rX' / 'goto' / 'if'
  std::string startToken { tokens[1] };

  if (startToken == ReturnOperator::keyword) {
    return ParseLib::parseReturn(tokens, lineNumber);
  } else if (startToken == GotoOperator::keyword) {
    return ParseLib::parseGoto(tokens, lineNumber);
  } else if (startToken == IfOperator::keyword) {
    return ParseLib::parseIf(tokens, lineNumber);
  }

  if (startToken.front() == RegisterToken) {
    return ParseLib::parseAssignment(tokens, lineNumber);
  }

  throw ParseException("No valid syntax detected", lineNumber);
}

const std::vector<int> Parser::parseDeclarationLine(std::string_view line) {
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
      std::string registerNumberStr { "" };
      while(line[idx] != RegisterDelimiter && line[idx] != EndBracketToken) {
        registerNumberStr += line[idx];
        idx++;
      }
      int n = ParseLib::getRegisterNumber(registerNumberStr, 0);
      registerNumbers.push_back(n);
    }
    idx++;
  }
  return registerNumbers; 
}

const std::vector<std::shared_ptr<Operator>> Parser::parse(const std::vector<std::string> lines) {
  std::vector<std::shared_ptr<Operator>> operators {};
  for (int i = 1; i < lines.size(); i++) {
    int lineLength { ParseLib::getLineLength(lines[i], i) };
    std::shared_ptr<Operator> o { parseLine(lines[i], lineLength, i) };
    operators.push_back(o);
  }
  return operators;
}

ParseException::ParseException(std::string_view error, int lineNumber)
  : error{error}, lineNumber{lineNumber} {}

const int ParseException::getLineNumber() const {
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