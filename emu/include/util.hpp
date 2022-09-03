#pragma once
#include <string>
#include <vector>
#include <sys/ioctl.h>

std::string repeat(std::string str, int n);
std::string concat(std::vector<std::string> list );
template<typename ... Args>
const char* string_format( const std::string& format, Args ... args );

winsize get_window_size();
std::vector<int> findMultiples(int x, int y, int f, int t);
