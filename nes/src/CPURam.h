#ifndef CPURAM_H
#define CPURAM_H

#include <stdint.h>
#include <vector>
#include "CPU.h"
#include "PPU.h"

#include <iostream>

class CPU;

class CPURam
{
   public:
   CPURam(CPU* cp, PPU* pp);
   void write(uint16_t address, uint8_t data);
   uint8_t read(uint16_t address);
   void loadROM(std::vector<uint8_t*> rom);
   void saveState(std::ostream& os);
   void loadState(std::istream& os);
   void setController1(uint8_t state) { controller1State = state; }
   std::vector<uint16_t> findValue(uint8_t);
   std::vector<uint16_t> findValue(uint16_t);

   private:
   uint8_t mem[0x10001];
   bool controllerStrobe;
   uint8_t controller1;
   uint8_t controller1State;
   CPU* cpu;
   PPU* ppu;

   uint32_t decode(uint16_t, bool w = false, uint8_t d = 0); //returns uint32_t because we sometime want 0x10000 (to be fixed)
};

#endif
