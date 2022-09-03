#pragma once
#include <regex>
#include <vector>
#include <map>
#include <string>
#include <cinttypes>
typedef std::uint8_t BYTE;


namespace Lexer {

class Token {
public:
  int type;
  std::vector<Token> children;
  std::string info;

  Token(){}
  Token(int type) {
    this->type=type;
    this->children={};
  }
  Token(int type,std::vector<Token> children) {
    this->type=type;
    this->children=children;
  }
  Token(int type,std::string info, std::vector<Token> children) {
    this->type=type;
    this->info=info;
    this->children=children;
  }
  std::string toString();
};

extern std::map<int, std::string> TokenTypeStr;

enum TokenType {
  VAR_DEF,
  SUBROUTINE,
  INSTRUCTION,
  LITERAL,
  VAR_NAME,
  ADDR,
};

namespace Regexes {
  extern const std::regex thing;
  extern const std::regex var_def;
  extern const std::regex sub;
  extern const std::regex inst;
  extern const std::regex literal;
  extern const std::regex addr;
};

Token tokenize(std::string str);

bool isVarDef    (const std::string str);
bool isSubroutine(const std::string str);
bool isInstruction(const std::string str);
bool isLiteral(const std::string str);
bool isAddr(const std::string str);


};
