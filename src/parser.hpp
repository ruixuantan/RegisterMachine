#include "operators.hpp"

#include <string>
#include <vector>

class Parser {
  private:
    Operator* parseLine(std::string &line);
  public:
    std::vector<int> parseDeclarationLine(std::string &line);
    std::vector<Operator*> parse(std::vector<std::string> lines); 
};