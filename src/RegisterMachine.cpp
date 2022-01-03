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

  try {
    const std::vector<int> args { parser.parseInitialArgs(argc, argv) };
    const std::vector<std::string> lines { IOReader::readFile(filename) };
    const std::vector<int> initialRegisters { parser.parseDeclarationLine(lines[0])};
      
    reg.setRegisters(args, initialRegisters);
    const std::vector<std::shared_ptr<Operator>> operators { parser.parse(lines) };
    const int returnRegister { ex.execute(operators, reg) };
    IOReader::printSuccess(reg.printRegister(returnRegister));

  } catch (const ParseArgsException& e) {
    IOReader::printArgsError(e.what());
  } catch (const ParseException& e) {
    IOReader::printCompilationError(e.what(), e.getLineNumber());
  } catch (const RuntimeException& e) {
    IOReader::printRuntimeError(e.what());
  } catch (const RegisterException& e) {
    IOReader::printRegisterError(e.what(), e.getRegNumber());
  }
  return 0;
}
