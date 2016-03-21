#ifndef PPURAM_H
#define PPURAM_H

#include <stdint.h>
#include <vector>

class PPURam
{
   public:
   PPURam();
   void write(uint16_t address, uint8_t data);
   uint8_t read(uint16_t address) const;
   void loadROM(std::vector<uint8_t*> rom);

   private:
   uint8_t mem[0x4000];

   uint16_t decode(uint16_t) const;
};

#endif
