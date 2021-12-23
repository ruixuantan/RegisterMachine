#include "ioreader.hpp"
#include "parser.hpp"
#include "register.hpp"
#include "operators.hpp"
#include "executor.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <iterator>

int main(int argc, char **argv) {
  std::string filename { argv[1] };
  Register reg{};
  Parser parser{};
  Executor ex{};

  std::vector<int> args { IOReader::readInitialArgs(argc, argv) };
  std::vector<std::string> lines { IOReader::readFile(filename) };
  std::vector<int> initialRegisters { parser.parseDeclarationLine(lines[0])};
  
  reg.setRegisters(args, initialRegisters);

  std::vector<Operator*> operators = parser.parse(lines);
  int returnRegister { ex.execute(operators, reg) };

  if (returnRegister == TERMINATE_LINE_NUMBER) {
    IOReader::print("Program terminated with an error");
  } else {
    IOReader::print("Program terminated successfully\n" + reg.printRegister(returnRegister));
  }

  return 0;
}
