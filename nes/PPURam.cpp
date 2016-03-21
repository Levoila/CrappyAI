#include "PPURam.h"


PPURam::PPURam()
{
   for(unsigned int i = 0; i < 0x4000; ++i) {
      mem[i] = 0xFF;
   }
}

void PPURam::loadROM(std::vector<uint8_t*> rom)
{

}

uint8_t PPURam::read(uint16_t address) const
{
   return mem[decode(address)];
}

void PPURam::write(uint16_t address, uint8_t byte)
{
   mem[decode(address)] = byte;
}

uint16_t PPURam::decode(uint16_t address) const
{
   return address;
}
