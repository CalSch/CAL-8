#include <regex>
#include "lexer.hpp"
#include "util.hpp"

const std::regex Lexer::Regexes::thing (R"~(fe)~");

const std::regex Lexer::Regexes::var_def (R"~(\w+: *\$?0x[0-9a-fA-F]{2})~");
const std::regex Lexer::Regexes::literal (R"~(\$0x[0-9a-fA-F]+)~");
const std::regex Lexer::Regexes::addr    (R"~(0x[0-9a-fA-F]+)~");
const std::regex Lexer::Regexes::sub     (R"~(\w+:)~");
const std::regex Lexer::Regexes::inst    (R"~((LDA|STA|ADD|SUB|JUMP|JSUB|RET|COMP|BIE|BNE|BIG|BIL)( \$?\w+){0,2})~",std::regex::icase);

std::map<int,std::string> Lexer::TokenTypeStr={
  {Lexer::VAR_DEF,"VAR_DEF"},
  {Lexer::SUBROUTINE,"SUBROUTINE"},
  {Lexer::LITERAL,"LITERAL"},
  {Lexer::INSTRUCTION,"INSTRUCTION"},
  {Lexer::VAR_NAME,"VAR_NAME"},
  {Lexer::ADDR,"ADDR"},
};

std::string Lexer::Token::toString() {
  std::string str;

  str+=Lexer::TokenTypeStr.count(type)==0 ? "UNDEFINED" :  Lexer::TokenTypeStr.at(type);
  str+="( ";
  if (info.length()!=0) str+=info;
  if (children.size()!=0) {
    if (info.length()!=0) str+=" , ";
    for (size_t i=0;i<children.size();i++) {
      str+=children.at(i).toString();
      str+=" ";
    }
  }
  str+=" )";

  return str;
}


Lexer::Token Lexer::tokenize(std::string str) {
  Lexer::Token token;
  str=trim(str);
  
  if (isSubroutine(str)) {
    token.type=Lexer::SUBROUTINE;
    token.info=trim(split(str,":")[0]);
  } else if (isInstruction(str)) {
    token.type=Lexer::INSTRUCTION;
    std::vector<std::string> list=split(str," ");
    token.info=trim(list.at(0));
    for (size_t i=1; i<list.size();i++) {
      std::string name=trim(list.at(i));
      if (isLiteral(name)) token.children.push_back(Lexer::Token(
                                Lexer::LITERAL,
                                name.substr(1,name.length()),
                                {}
                              ));
      else if (isAddr(name)) token.children.push_back(Lexer::Token(
                                Lexer::ADDR,
                                name,
                                {}
                              ));
      else {
            token.children.push_back(Lexer::Token(
                                Lexer::VAR_NAME,
                                name,
                                {}
                              ));
            // printf("  ---%s | L: %d A: %d---  ",name.c_str(),isLiteral(name),isAddr(name));
      }
    }
  } else if (isVarDef(str)) {
    token.type=Lexer::VAR_DEF;
    std::vector<std::string> list=split(str,":");
    token.children.push_back(Lexer::Token(
                               Lexer::VAR_NAME,
                               trim(list.at(0)),
                               {}
                             ));
    if (list.size()>1) {
      std::string name=trim(list.at(1));
      token.children.push_back(Lexer::Token(
                               Lexer::LITERAL,
                               name.substr(1,name.length()),
                               {}
                             ));
    }
  }

  return token;
}

bool Lexer::isVarDef(const std::string str) { return std::regex_match(str,Lexer::Regexes::var_def); }
bool Lexer::isSubroutine(const std::string str) { return std::regex_match(str,Lexer::Regexes::sub); }
bool Lexer::isInstruction(const std::string str) { return std::regex_match(str,Lexer::Regexes::inst); }
bool Lexer::isLiteral(const std::string str) { return std::regex_match(str,Lexer::Regexes::literal); }
bool Lexer::isAddr(const std::string str) { return (!isLiteral(str)) && std::regex_match(str,Lexer::Regexes::addr); }

