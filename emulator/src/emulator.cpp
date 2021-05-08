#include "emulator.hpp"

using namespace emu;

#define DBG_FUNCTION() DEBUG(std::cout << "Function " << __FUNCTION__ << std::endl;)
#define PRINT_MEM(size, loc) \
  DEBUG(for (unsigned int i = 0; i < size;) \
  {                                   \
    std::cout << "0x" << i << ": ";   \
    for (; i < size; i++)             \
    {                                 \
      std::cout << std::hex << static_cast<unsigned int>(*(loc+i)) << " ";   \
    }                                 \
    std::cout << std::endl;           \
  })
#define TMOD() (*m_state.regs.TMOD)
#define TCON() (*m_state.regs.TCON)
#define IE() (*m_state.regs.IE)
#define GET_TIMER0_MODE() (TMOD() & 0x03)
#define GET_TIMER1_MODE() ((TMOD() & 0x30) >> 4)
#define CHECK_TIMER0_ON() ((TCON() & 0x10) != 0)
#define CHECK_TIMER1_ON() ((TCON() & 0x40) != 0)
#define SET_INTERRUPT(mask) *(m_state.regs.TCON) |= mask
#define CALL_INT(address, mask) *m_state.regs.TCON &= mask; m_opHandler->jumpInterrupt(address)
#define DBG_TIMER(number) std::hex << "Timer " << number << ": Mode " << CAST((TMOD() >> (4 * number)) & 0x0F) \
    << " TL" << number << "=0x" << CAST(*(m_state.regs.TL0 + number)) << " TH" << number << "=0x" << CAST(*(m_state.regs.TL0 + number + 2))
#define DBG_TIMERS() DEBUG(TIMER0(std::cout << DBG_TIMER(0) << std::endl;) TIMER1(std::cout << DBG_TIMER(1) << std::endl;))

Emulator::Emulator(std::function<void(state_t&)> stepCallback)
{
  m_stepCallback = stepCallback;
  m_state.program_memory = new uint8_t[MEM_SIZE];
  m_state.external_memory = new uint8_t[MEM_SIZE];
  m_state.internal_data = new uint8_t[INTERNAL_MEM_SIZE];
  m_state.sfr_memory = new uint8_t[SFR_MEM_SIZE];
  m_opHandler = std::make_unique<OpcodeHandler>(m_state);
  m_jpTable[0] = &Emulator::timer_mode_00;
  m_jpTable[1] = &Emulator::timer_mode_01;
  m_jpTable[2] = &Emulator::timer_mode_10;
  m_jpTable[3] = &Emulator::timer_mode_11;
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
  fuint32_t power_config = (*m_state.regs.PCON) & 0x03;
  if ((power_config & 0x02) != 0) return; // power down bit set
  if (power_config == 0){ m_opHandler->execOpcode(); } // otherwise in idle mode (can be stopped through interrupts)
  DBG_TIMERS()
  TIMER0(if (CHECK_TIMER0_ON()){ (*this.*(m_jpTable[GET_TIMER0_MODE()]))(m_state.regs.TL0, 0x20); })
  TIMER1(if (CHECK_TIMER1_ON()){ (*this.*(m_jpTable[GET_TIMER1_MODE()]))(m_state.regs.TL1, 0x80); })
  m_stepCallback(m_state);
  handleInterrupts();
}


void Emulator::triggerExternalInterrupt0()
{
  *m_state.regs.IE |= 0x01;

}

void Emulator::triggerExternalInterrupt1()
{
  *m_state.regs.IE |= 0x04;
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
  if (IE() & 0x80 == 0 || IE() & 0x1F == 0) return; // either interrupt master disabled or no interrupt enabled
  uint8_t ints = TCON() & 0xAA;
  if (ints == 0) return;

  DEBUG(std::cout << "Interrupt: TCON 0b" << std::bitset<8>{ints} << std::endl;)

  // currently priorities are ignored
  if ((ints & 0xA0) != 0)
  { // handle timer interrupts
    if ((ints & 0x80) != 0)
    { // timer 1
      CALL_INT(0x1B, 0x7F);
    }
    else
    { // timer 0
      CALL_INT(0x0B, 0xDF);
    }
  }
  else
  { // handle external interrupts
    if ((ints & 0x08) != 0)
    { // external 1
      CALL_INT(0x13, 0xF7);
    }
    else
    { // external 0
      CALL_INT(0x03, 0xFD);
    }
  }
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

void Emulator::timer_mode_00(uint8_t *timer, uint8_t mask)
{
  uuint16_t timer_val;
  timer_val.v8[0] = *timer & 0x1F; // mask away upper 3 bit
  timer_val.v8[1] = *(timer + 2);
  uint16_t old = timer_val.v16;
  timer_val.v8[0] += m_state.last_opcode;

  if ((timer_val.v8[0] & 0xE0) != 0)
  {
    timer_val.v8[0] &= 0x1F;
    timer_val.v8[1]++;
  }

  if (timer_val.v16 < old) // overflow
  {
    SET_INTERRUPT(mask);
    *timer = 0;
    *(timer + 2) = 0;
  }
  else
  {
    *timer = timer_val.v8[0];
    *(timer + 2) = timer_val.v8[1];
  }
}

void Emulator::timer_mode_01(uint8_t *timer, uint8_t mask)
{
  uuint16_t timer_val;
  timer_val.v8[0] = *timer;
  timer_val.v8[1] = *(timer + 2);
  uint16_t old = timer_val.v16;
  timer_val.v16 += m_state.last_opcode;
  if (timer_val.v16 < old) // overflow
  {
    SET_INTERRUPT(mask);
  }
  *timer = timer_val.v8[0];
  *(timer + 2) = timer_val.v8[1];
}

void Emulator::timer_mode_10(uint8_t *timer, uint8_t mask)
{
  uint8_t tl = *timer;
  uint8_t old = tl;
  tl += m_state.last_opcode;
  if (tl < old) // overflow
  {
    *timer = *(timer + 2) + tl;
    SET_INTERRUPT(mask);
  }
  else
  {
    *timer = tl;
  }
}

void Emulator::timer_mode_11(uint8_t *timer, uint8_t mask)
{
  if (timer == m_state.regs.TL1) return;
  uint8_t tim = *timer;
  uint8_t old = tim;
  tim += m_state.last_opcode;
  if (tim < old)
  {
    SET_INTERRUPT(mask);
  }
  *timer = tim;
  tim = *(timer + 2);
  old = tim;
  tim += m_state.last_opcode;
  if (tim < old)
  {
    SET_INTERRUPT(mask);
  }
  *(timer + 2) = tim;
}
