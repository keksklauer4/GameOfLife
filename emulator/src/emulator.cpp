#include "emulator.hpp"

using namespace emu;

Emulator::Emulator(const std::string& filename)
{
  m_state.program_memory = new uint8_t[MEM_SIZE];
  m_state.external_memory = new uint8_t[MEM_SIZE];
  m_state.setRegisters();
  m_state.reset();
  m_opHandler = std::make_unique<OpcodeHandler>(m_state);
}


Emulator::~Emulator()
{


}


void Emulator::step()
{

}


void Emulator::triggerExternalInterrupt()
{

}


void Emulator::init()
{

}



void Emulator::handleInterrupts()
{

}



void Emulator::readFile()
{

}