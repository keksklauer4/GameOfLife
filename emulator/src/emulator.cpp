#include "emulator.hpp"

using namespace emu;

#define PRINT_MEM(size, loc) \
  for (unsigned int i = 0; i < size;) \
  {                                   \
    std::cout << "0x" << i << ": ";   \
    for (; i < size; i++)             \
    {                                 \
      std::cout << std::hex << static_cast<unsigned int>(*(loc+i)) << " ";   \
    }                                 \
    std::cout << std::endl;           \
  }

Emulator::Emulator()
{
  m_state.program_memory = new uint8_t[MEM_SIZE];
  m_state.external_memory = new uint8_t[MEM_SIZE];
  m_state.internal_data = new uint8_t[INTERNAL_MEM_SIZE];
  m_state.sfr_memory = new uint8_t[SFR_MEM_SIZE];
  m_opHandler = std::make_unique<OpcodeHandler>(m_state);
}


Emulator::~Emulator()
{
  if (m_state.program_memory != nullptr) delete[] m_state.program_memory;
  if (m_state.external_memory != nullptr) delete[] m_state.external_memory;
  if (m_state.internal_data != nullptr) delete[] m_state.internal_data;
  if (m_state.sfr_memory != nullptr) delete[] m_state.sfr_memory;

}

void Emulator::setup(const std::string& filename)
{
  init();
  readFile(filename);
}

void Emulator::step()
{
  m_opHandler->execOpcode();
  handleInterrupts();
}


void Emulator::triggerExternalInterrupt()
{

}


void Emulator::init()
{
  std::memset(m_state.program_memory, 0, MEM_SIZE);
  std::memset(m_state.external_memory, 0, MEM_SIZE);
  std::memset(m_state.internal_data, 0, INTERNAL_MEM_SIZE);
  std::memset(m_state.sfr_memory, 0, SFR_MEM_SIZE);
  m_state.setRegisters();
  m_state.reset();
}



void Emulator::handleInterrupts()
{

}



void Emulator::readFile(const std::string& filename)
{
  std::ifstream binary(filename, std::ios::binary);
  if (!binary.is_open()) throw std::runtime_error("Could not load file.");

  binary.seekg(0, std::ios::end);
  size_t length = binary.tellg();
  std::cout << "Length of file: " << length << "bytes. " << std::endl;
  if (length > ADDRESSABLE) {
    binary.close();
    throw std::out_of_range("File too big to load in.");
  }

  binary.seekg(std::ios::beg);
  binary.read((char*)m_state.program_memory, length);
  binary.close();
  PRINT_MEM(length, m_state.program_memory)
}