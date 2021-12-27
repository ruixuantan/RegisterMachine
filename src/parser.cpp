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
  const int getRegisterNumber(const std::string& registerStr, const int& lineNumber) {
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
  const int getNumber(const std::string& numberStr, const int& lineNumber) {
    try {
      return std::stoi(numberStr);
    } catch (std::invalid_argument) {
      throw ParseException("Number passed is not an integer", lineNumber);
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
  const int parseNumberToEnd(const std::string_view line, const int& length, int idx, const int& lineNumber) {
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
   * @brief Increments idx to the index of the token right after '<line number>. '
   * Expects a '.' to terminate the line.
   */
  const int parseLineNumber(const std::string_view line, const int& length, int idx, const int& lineNumber) {
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
  const std::string getToken(const std::string_view line, const int& length, int &idx, const int& lineNumber) {
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

  /**
   * @brief Gets the register number of a return operator. 
   * idx should be at the position of 'r'
   */
  const int parseReturn(const std::string_view line, const int& length, int idx, const int& lineNumber) {
    std::string reg { "" };
    for(int i = idx; i < length; i++) {
      reg += line[i];
    }
    return getRegisterNumber(reg, lineNumber);
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

  const std::unique_ptr<BinaryOperator> parseAssignment(const std::string_view line, const int& length, int idx, const int& lineNumber) {
    while(line[idx] != DelimiterToken) {
      idx++;
    }
    idx++;
  
    std::string lhs {""};
    while(line[idx] != DelimiterToken && idx < length) {
      lhs += line[idx];
      idx++;
    }
    Variable lhsVar {parseVariable(lhs, lineNumber)};
    Variable rhsVar;

    if (idx == length) {
      return std::unique_ptr<AddOperator>(new AddOperator(lhsVar, rhsVar));
    }

    idx++;
    std::string op {line[idx]};
    std::string rhs {""};
    for (int i = idx + 2; i < length; i++) {
      rhs += line[i];
    }
    rhsVar = parseVariable(rhs, lineNumber); 

    if (op == AddOperator::keyword) {
      return std::unique_ptr<AddOperator>(new AddOperator(lhsVar, rhsVar)); 
    } else if (op == SubtractOperator::keyword) {
      return std::unique_ptr<SubtractOperator>(new SubtractOperator(lhsVar, rhsVar)); 
    } else {
      throw ParseException("Operator is not recognised", lineNumber);
    }
  }

  const std::vector<std::string> tokenise(const std::string_view line, const int& length, int idx) {
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

  IfOperator* parseIf(const std::string_view line, const int& length, int idx, const int& lineNumber) {
    std::vector<std::string> ls {tokenise(line, length, idx)};
    Variable lhs {parseVariable(ls[0], lineNumber)};
    Variable rhs {parseVariable(ls[2], lineNumber)};

    if (ls[3] != GotoOperator::keyword) {
      throw ParseException("Goto operator expected", lineNumber);
    }

    int returnLine {getNumber(ls[4], lineNumber)};
    
    if (ls[1] == LessThanOperator::keyword) {
      return new IfOperator(returnLine, std::unique_ptr<LessThanOperator>(new LessThanOperator(lhs, rhs)));
    } else if (ls[1] == EqualityOperator::keyword) {
      return new IfOperator(returnLine, std::unique_ptr<EqualityOperator>(new EqualityOperator(lhs, rhs)));
    } else {
      throw ParseException("Comparison operator expected", lineNumber);
    }
  }
}

Operator* Parser::parseLine(const std::string_view line, const int& length, const int& lineNumber) {
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
    int number = ParseLib::parseNumberToEnd(line, length, idx, lineNumber);
    return new GotoOperator{number};
  } else if (token == IfOperator::keyword) {
    // Expecting ' rX/X </== rX/X goto X'
    return ParseLib::parseIf(line, length, idx, lineNumber);
  }

  if (token.front() == RegisterToken) {
    // Expecting ' = X/rX + X/rX'
    int regNumber { ParseLib::getRegisterNumber(token, lineNumber) };
    return new AssignmentOperator{
      regNumber, ParseLib::parseAssignment(line, length, idx + 1, lineNumber)};
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

const std::vector<Operator*> Parser::parse(const std::vector<std::string> lines) {
  std::vector<Operator*> operators {};
  for (int i = 1; i < lines.size(); i++) {
    int lineLength { ParseLib::getLineLength(lines[i], i) };
    Operator *o { parseLine(lines[i], lineLength, i) };
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