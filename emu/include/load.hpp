#pragma once
#include <vector>
#include <cinttypes>
#include "load.hpp"
typedef std::uint8_t BYTE;

std::vector<BYTE> load(unsigned int offset, unsigned int length, char const* filename);