#include "parser.hpp"
#include "operators.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <exception>

const char BeginBracketToken = '(';
const char EndBracketToken = ')';
const char RegisterDelimiter = ',';
const char LineNumberToken = '.';
const char RegisterToken = 'r';
const char DelimiterToken = ' ';
const char CommentToken = '#';

const int EMPTY_LINE_NUMBER = -1;

std::vector<int> Parser::parseInitialArgs(int argc, char **argv) {
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
  int getLineLength(std::string &line, int lineNumber) {
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
  int getRegisterNumber(std::string &registerStr, int lineNumber) {
    try {
      return std::stoi(registerStr.substr(1, registerStr.size()));
    } catch (std::invalid_argument) {
        throw ParseException("Register number passed is invalid", lineNumber);
    }
  }

  /**
   * @brief Converts the string from idx to the end, to a number
   * 
   * @param line 
   * @param length effective length of the string
   * @param idx the starting index to parse
   * @param lineNumber current line number being parsed
   * @return int 
   */
  int parseNumber(std::string &line, int length, int idx, int lineNumber) {
    std::string numberStr {""};
    for (int i = idx; i < length; i++) {
      numberStr += line[i];
    }
    try {
      return std::stoi(numberStr);
    } catch (std::invalid_argument) {
      throw ParseException("Number passed is not an integer", lineNumber);
    }
  }

  /**
   * @brief Parses the line number.
   * Expects a '.' to terminate the line.
   */
  int parseLineNumber(std::string &line, int length, int idx, int lineNumber) {
    for (int i = idx; i < length; i++) {
      if (line[i] == LineNumberToken) {
        i += 2;
        return i;
      }
    }
    throw ParseException("Line number did not terminate with '.'", lineNumber);
  }

  /**
   * @brief Get the token from the string.
   * 
   * @param line the current line being parsed
   * @param length the effective length of the string
   * @param idx the starting index to being parsing
   * @param lineNumber current line being parsed
   * @return std::string 
   */
  std::string getToken(std::string &line, int length, int &idx, int lineNumber) {
    std::string token{""};
    for (int i = idx; i < length; i++) {
      if (line[i] == DelimiterToken) {
        idx = i + 1;
        return token;
      }
      token += line[i];
    }
    throw ParseException("Whitespaces not detected", lineNumber); 
  }

  int parseReturn(std::string &line, int length, int idx, int lineNumber) {
    std::string reg { "" };
    for(int i = idx; i < length; i++) {
      reg += line[i];
    }
    return getRegisterNumber(reg, lineNumber);
  }

  int parseAssignment(std::string &line, int length, int idx, int lineNumber) {
    std::string reg {""};
    while(line[idx] != DelimiterToken) {
      idx++;
    }
    for(int i = idx + 1; i < length; i++) {
      reg += line[i];
    }
    try {
      return std::stoi(reg);
    } catch (std::invalid_argument) {
      throw ParseException("Integer is not passed to the register", lineNumber);
    }
  }
}

Operator* Parser::parseLine(std::string &line, int length, int lineNumber) {
  int idx { 0 };
  idx = ParseLib::parseLineNumber(line, length, idx, lineNumber);

  // Tokens could be either 'return' or 'rX'
  std::string token = ParseLib::getToken(line, length, idx, lineNumber);

  if (token == ReturnOperator::keyword) {
    // Expecting ' rX'
    int toReturn = ParseLib::parseReturn(line, length, idx, lineNumber);
    return new ReturnOperator{toReturn};
  } else if (token == GotoOperator::keyword) {
    // Expecting ' X'
    int number = ParseLib::parseNumber(line, length, idx, lineNumber);
    return new GotoOperator{number};
  } 

  if (token.front() == RegisterToken) {
    // Expecting ' = X'
    int regNumber { ParseLib::getRegisterNumber(token, lineNumber) };
    int value = ParseLib::parseAssignment(line, length, idx + 1, lineNumber);
    return new AssignmentOperator{regNumber, value};
  }

  throw ParseException("No valid syntax detected", lineNumber);
}

std::vector<int> Parser::parseDeclarationLine(std::string &line) {
  int length = line.size();
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

std::vector<Operator*> Parser::parse(std::vector<std::string> lines) {
  std::vector<Operator*> operators {};
  for (int i = 1; i < lines.size(); i++) {
    int lineLength { ParseLib::getLineLength(lines[i], i) };
    Operator *o { parseLine(lines[i], lineLength, i) };
    operators.push_back(o);
  }
  return operators;
}

ParseException::ParseException(std::string error, int lineNumber)
  : error{error}, lineNumber{lineNumber} {}

int ParseException::getLineNumber() const {
  return this->lineNumber;
}

const char* ParseException::what() const noexcept {
  return this->error.c_str();
}

ParseArgsException::ParseArgsException(std::string error)
  : error{error} {}

const char* ParseArgsException::what() const noexcept {
  return this->error.c_str();
}