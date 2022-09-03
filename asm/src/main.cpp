#include <stdio.h>
#include <vector>
#include <cinttypes>
#include <string>
#include <math.h>
#include <map>
#include "lexer.hpp" 
#include "opcodes.hpp"
#include "util.hpp"
typedef std::uint8_t BYTE;
typedef std::uint8_t ADDR;

// Definitions
const int ram_size=256;


std::map<std::string,ADDR> vars;

std::vector<std::string> fileLines;


BYTE ram[ram_size];
int main() {

  fileLines=loadFile("/home/calvin/Coding/C++/CAL-8/asm/program.casm");

	printf("Program loaded.\n");
  printf("%s\n",trim("  hel lo        ").c_str());

  
  for (long unsigned int i=0; i<fileLines.size(); i++) {
    std::string line=fileLines.at(i);
    if (line.find(";")!=std::string::npos) {
      line=split(line,";").at(0);
    }
    if (trim(line).length()==0) continue;
    printf("%s%s |  %s\n",line.c_str(),repeat(" ",16-line.length()).c_str(),Lexer::tokenize(line).toString().c_str());
  }
  
  return 0;
}
