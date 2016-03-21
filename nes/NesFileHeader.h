#ifndef NESFILEHEADER_H
#define NESFILEHEADER_H

#include <stdint.h>
#include <fstream>
#include <assert.h>
#include <vector>

enum Mirroring  {VERTICAL, HORIZONTAL, FOUR_SCREEN, SINGLE_SCREEN};

class NesFileHeader
{
   public:
      NesFileHeader(std::ifstream& file);
      uint8_t getMapper() const { return mapper; }
      Mirroring getMirroring() const { return mirroring; }
      bool hasBatteryBackedSRAM() const { return batteryBackedSRAM; };
      std::vector<uint8_t*> getPRGROM() const { return PRGROM; }
      std::vector<uint8_t*> getCHRROM() const { return CHRROM; };
   private:
      uint8_t mapper;
      std::vector<uint8_t*> PRGROM;
      std::vector<uint8_t*> CHRROM;
      Mirroring mirroring;
      bool batteryBackedSRAM;
};

#endif
