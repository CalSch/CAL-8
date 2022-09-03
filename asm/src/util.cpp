#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdexcept>
#include "util.hpp"


const std::string repeat(std::string str, int n) {
  	std::string repeat;

  	for (int i = 0; i < n; i++) {
  		repeat += str;
  	}

  	return repeat;
}


const std::string concat(std::vector<std::string> list ) {
	std::string str;
	for (long unsigned int i=0;i<list.size();i++) {
		str+=list.at(i);
	}

	return str;
}


const std::string& toString (const char* s) {
    return *(new std::string(s));
}

std::string& toString(char s) {
  return *(new std::string(1,s));
}


// trim from start (in place)
const std::string ltrim(std::string s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(),
          std::not1(std::ptr_fun<int, int>(std::isspace))));
  return s;
}

// trim from end (in place)
const std::string rtrim(std::string s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
          std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
  return s;
}

const std::string trim(std::string str) {
  return ltrim(rtrim(str));
}

const std::vector<std::string> split(std::string str, std::string deli) {
  std::vector<std::string> list;
  size_t pos = 0;
  std::string token;
  while ((pos = str.find(deli)) != std::string::npos) {
    token = str.substr(0, pos);
    list.push_back(token);
    str.erase(0, pos + deli.length());
  }
  list.push_back(str);

  return list;
}


template<typename ... Args>
const std::string string_format( const std::string& format, Args ... args ) {
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}


winsize get_window_size() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

   	return w;
}

std::vector<std::string> loadFile(std::string filename) {
  std::ifstream file (filename);
  std::vector<std::string> lines;
  if (file.is_open()) {
    std::string line;
    while (std::getline(file,line)) {
      lines.push_back(line);
    }
    file.close();

    return lines;
  }
  throw std::runtime_error(string_format("File \"%s\" not found",filename.c_str()));
  return {};
}
