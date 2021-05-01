#include <iostream>
#include <memory>

#include "emulator.hpp"
using namespace emu;
int main()
{
  std::cout << "hi" << std::endl;
  std::string filename = "../test/test.bin";
  Emulator emu;
  emu.setup(filename);

  for(int i = 0; i < 100; i++)
  {
    std::cout << "Iteration " << (i+1) << std::endl;
    emu.step();
    std::cout << std::endl << std::endl;
  }

}