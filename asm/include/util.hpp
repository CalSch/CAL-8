#pragma once
#include <string>
#include <vector>
#include <sys/ioctl.h>

const std::string repeat(const std::string str, int n);
const std::string concat(const std::vector<std::string> list );
const std::vector<std::string> split(const std::string str, const std::string deli);
const std::string ltrim(const std::string str);
const std::string rtrim(const std::string str);
const std::string trim(const std::string str);
template<typename ... Args>
const std::string string_format( const std::string& format, Args ... args );

winsize get_window_size();

std::vector<std::string> loadFile(std::string filename);
