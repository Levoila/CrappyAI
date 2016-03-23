#include "VRAM.h"
#include <algorithm>
#include "string.h"
#include <iostream>
#include <iomanip>

using namespace std;

VRAM::VRAM()
	: CHROffset(0)
{}

VRAM::~VRAM()
{
	delete[] mem;
}

void VRAM::write(uint16_t addr, uint8_t data)
{
	if(addr < 0x2000) {
		return;
	}
   mem[decode(addr)] = data;
}

uint8_t VRAM::read(uint16_t addr) const
{
   return mem[decode(addr)];
}

void VRAM::load(const std::vector<uint8_t*>& data)
{
	if(data.size() == 0) {
		mem = new uint8_t[0x4000];
		memSize = 0x4000;
	} else {
		mem = new uint8_t[0x2000 + 0x2000 * data.size()];
		memSize = 0x2000 + 0x2000 * data.size();
	}
	
	if(data.size() != 0) {
		memcpy(mem, data[0], 0x2000);
	}
	for(int i = 1; i < data.size(); ++i) {
		memcpy(mem+0x2000 + 0x2000 * i, data[i], 0x2000);
	}
}

void VRAM::saveState(std::ostream& os)
{
	os.write((char*)(&CHROffset), sizeof(CHROffset));
	os.write((char*)(&mirroring), sizeof(mirroring));

	os.write((char*)mem, sizeof(char) * memSize);
}

void VRAM::loadState(std::istream& os)
{
	os.read((char*)(&CHROffset), sizeof(CHROffset));
	os.read((char*)(&mirroring), sizeof(mirroring));

	os.read((char*)mem, sizeof(char) * memSize);
}

uint16_t VRAM::decode(uint16_t addr) const
{
   if(addr < 0x2000) { //Pattern tables
	   addr += CHROffset;
   } else if(addr < 0x3000) { //Name Tables and attribute tables
      if(mirroring == HORIZONTAL) {
         if(addr < 0x2800) {
            addr &= 0x23FF;
         } else {
            addr &= 0x2BFF;
         }
      } else if(mirroring == VERTICAL && addr >= 0x2800) {
         addr -= 0x800;
      } else if(mirroring == SINGLE_SCREEN) {
         addr &= 0x23FF;
      }
   } else if(addr < 0x3F00) { //Name tables mirror
      addr = decode(addr - 0x1000);
   } else if(addr < 0x3F10) { //BG palette

   } else if(addr < 0x3F20) { //Sprite palette
      if(addr % 4 == 0) {
         addr -= 0x10;
      }
   } else { //Palette mirror
      addr = decode(addr & 0x3F1F);
   }
   return addr;
}
