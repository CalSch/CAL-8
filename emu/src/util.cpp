#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include "util.hpp"


std::string repeat(std::string str, int n) {
  	std::string repeat;

  	for (int i = 0; i < n; i++) {
  		repeat += str;
  	}

  	return repeat;
}


std::string concat(std::vector<std::string> list ) {
	std::string str;
	for (long unsigned int i=0;i<list.size();i++) {
		str+=list.at(i);
	}

	return str;
}


template<typename ... Args>
const char* string_format( const std::string& format, Args ... args ) {
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ).c_str(); // We don't want the '\0' inside
}


winsize get_window_size() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

   	return w;
}


