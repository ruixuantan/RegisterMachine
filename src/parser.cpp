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

int getRegisterNumber(std::string registerStr) {
  return stoi(registerStr.substr(1, registerStr.size()));
}

int parseReturn(std::string &line, int length, int idx) {
  std::string reg { "" };
  for(int i = idx; i < length; i++) {
    reg += line[i];
  }
  return getRegisterNumber(reg);
}

int parseAssignment(std::string &line, int length, int idx) {
  std::string reg {""};
  while(line[idx] != DelimiterToken) {
    idx++;
  }
  for(int i = idx + 1; i < length; i++) {
    reg += line[i];
  }
  return stoi(reg);
}

Operator* Parser::parseLine(std::string &line) {
  int idx { 0 };
  int length = line.size();

  while(line[idx] != LineNumberToken) {
    idx++;
  }
  idx += 2;

  // Tokens could be either 'return' or 'rX'
  std::string token { "" };
  while(line[idx] != DelimiterToken) {
    token += line[idx];
    idx++;
  }

  if (token == ReturnOperator::keyword) {
    // Expecting ' rX'
    int toReturn = parseReturn(line, length, idx + 1);
    return new ReturnOperator{toReturn};
  } 

  if (token.front() == RegisterToken) {
    // Expecting ' == X'
    int regNumber { getRegisterNumber(token) };
    int value = parseAssignment(line, length, idx + 1);
    return new AssignmentOperator{regNumber, value};
  }

  return new ReturnOperator{TERMINATE_LINE_NUMBER};
}

std::vector<int> Parser::parseDeclarationLine(std::string &line) {
  int idx { 0 };
  std::vector<int> registerNumbers {};
  
  while (line[idx] != BeginBracketToken) {
    idx++;
  }

  while (line[idx] != EndBracketToken) {
    idx++;
    if (line[idx] == RegisterToken) {
      idx++;
      std::string registerNumberStr { "" };
      while(line[idx] != RegisterDelimiter && line[idx] != EndBracketToken) {
        registerNumberStr += line[idx];
        idx++;
      }
      registerNumbers.push_back(stoi(registerNumberStr));
    }
  }

  return registerNumbers;
}

std::vector<Operator*> Parser::parse(std::vector<std::string> lines) {
  std::vector<Operator*> operators {};
  for (int i = 1; i < lines.size(); i++) {
    Operator *o = parseLine(lines[i]);
    operators.push_back(o);
  }
  return operators;
}