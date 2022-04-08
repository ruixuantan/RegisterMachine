#include "ioreader.hpp"
#include "parser.hpp"
#include "register.hpp"
#include "operators.hpp"
#include "executor.hpp"

#include <vector>
#include <array>
#include <iterator>

using namespace registers;
using namespace executor;
using namespace parser;
using namespace ioreader;

int main(int argc, char **argv) {
  std::string filename { argv[1] };
  Register reg{};

  try {
    const std::vector<size_t> args { Parser::parseInitialArgs(argc, argv) };
    const std::vector<std::string> lines { readFile(filename) };
    const std::vector<size_t> initialRegisters { Parser::parseDeclarationLine(lines[0])};
    reg.setRegisters(args, initialRegisters);
    const std::vector<std::shared_ptr<operators::Operator>> operators { Parser::parse(lines) };
    const size_t returnRegister { Executor::execute(operators, reg) };
    printSuccess(reg.printRegister(returnRegister));

  } catch (const ParseArgsException& e) {
    printArgsError(e.what());
  } catch (const ParseException& e) {
    printCompilationError(e.what(), e.getLineNumber());
  } catch (const RuntimeException& e) {
    printRuntimeError(e.what());
  } catch (const RegisterException& e) {
    printRegisterError(e.what(), e.getRegNumber());
  }
  return 0;
}
