#include <iostream>
#include <memory>
#include <chrono>

#include "emulator.hpp"

using namespace emu;
int main()
{
  std::cout << "hi" << std::endl;
  std::string filename = "../test/main.bin";
  Emulator emu;
  emu.setup(filename);

  using namespace std::chrono;
  high_resolution_clock::time_point t1 = high_resolution_clock::now();
  for(int i = 0; i < 1000000; i++)
  {
    DEBUG(std::cout << std::dec << "Iteration " << (i+1) << std::endl;)
    emu.step();
    //DEBUG(char c = getchar();)
    DEBUG(std::cout << std::endl << std::endl;)
  }
  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
  std::cout << "It took me " << time_span.count() << " seconds.";
  std::cout << std::endl;

  return 0;

}