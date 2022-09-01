#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iterator>
#include <cinttypes>
typedef std::uint8_t BYTE;

using Bytes = std::vector<BYTE>;

Bytes load(std::uint32_t      offset,
                 std::uint32_t      length,
                 char const* filename)
{
  // Not going to explicitly check these.
  // The use of gcount() below will compensate for a failure here.
  std::ifstream is(filename, std::ios::binary);
  is.seekg(offset);

  Bytes data(length);
  is.read(reinterpret_cast<char*>(data.data()), length);

  // We have to check that reading from the stream actually worked.
  // If any of the stream operation above failed then `gcount()`
  // will return zero indicating that zero data was read from the
  // stream.
  data.resize(is.gcount());

  // Simply return the vector to allow move semantics take over.
  return data;
}