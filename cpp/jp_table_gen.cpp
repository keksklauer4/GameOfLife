#include <fstream>
#include <cinttypes>
#include "config.hpp"

int main()
{
  std::ofstream file("jp_table.asm");

  file << ";" << std::endl
       << "; Created by keksklauer4 - 26.04.2021" << std::endl
       << ";" << std::endl << std::endl;

  file << "; Huge jump table to enable fast random access in external RAM." << std::endl
       << std::endl;

  file << "cseg at " << START_ADDR << std::endl;
  file << "l_jp_table:" << std::endl;

  for (uint32_t address = 0; address < 256; address++){
    file << std::endl;
    file << "; destination for A = " << address << ":" << std::endl;
    file << "   MOV DPTR, #" << address << std::endl;
    file << "   RET" << std::endl << std::endl;
  }

  file << std::endl << std::endl << "END" << std::endl;

  file.close();
}