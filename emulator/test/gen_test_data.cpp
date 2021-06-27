#include <fstream>
#include <stdexcept>
#include <string>

#include "test_data.hpp"
using namespace emu::testing;

int main()
{
  for (const auto& pair : data)
  {
    std::ofstream outFile(pair.first);
    if (!outFile.is_open())
    {
      throw std::runtime_error("Could not open file "+ pair.first);
    }

    outFile << pair.second << std::endl;
    outFile.close();
  }
}