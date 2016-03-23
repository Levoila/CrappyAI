#ifndef VRAM_H
#define VRAM_H

#include "NesFileHeader.h"
#include <vector>

class VRAM
{
   public:
      VRAM();
	  ~VRAM();
      void write(uint16_t addr, uint8_t data);
      uint8_t read(uint16_t addr) const;
      void load(const std::vector<uint8_t*>& data);
      void saveState(std::ostream& os);
      void loadState(std::istream& os);
      
      void setMirroring(Mirroring m) { mirroring = m; }

	  uint16_t CHROffset; //Public to be accessed by CPURam to test some mappers
   private:
      size_t memSize;
      uint8_t* mem;
      Mirroring mirroring;

      uint16_t decode(uint16_t addr) const;
};

#endif
