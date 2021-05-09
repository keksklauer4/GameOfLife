#include <iostream>
#include <memory>
#include <chrono>

#include "emulator.hpp"

using namespace emu;
using namespace std::chrono;

int main(int argc, char** argv)
{
  if (argc != 2 || argv == nullptr || *argv == nullptr) return -1;
  std::string filename(*(argv + 1));

  auto lambda = std::function<void(state_t&)>([](state_t& s)
    {
      //std::cout << "PORT0: " << std::hex << static_cast<unsigned int>(*(s.regs.P0)) << std::endl;
  });

  Emulator emu(lambda);
  emu.setup(filename);

  high_resolution_clock::time_point t1 = high_resolution_clock::now();
  for(int i = 0; i < 100000000; i++)
  {
    DEBUG(std::cout << std::dec << "Iteration " << (i+1) << std::endl;)
    emu.step();
    DEBUG(std::cout << std::endl << std::endl;)
  }
  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
  std::cout << "It took me " << time_span.count() << " seconds.";
  std::cout << "And I emulated " << emu.getState().cycles_passed << " Cycles." << std::endl;
  std::cout << std::endl;

  return 0;

}