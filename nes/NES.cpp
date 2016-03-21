#include "NES.h"

#include <fstream>
#include <iostream>

NES::NES()
	: ppu(new PPU()), cpu(new CPU(ppu)), ram(cpu, ppu)
{
	
}

void NES::loadROM(std::string filename)
{
	std::ifstream nesFile(filename.c_str(), std::ios::binary);

   if(!nesFile.is_open()) {
      std::cout << "Impossible d'ouvrir le fichier " << filename << std::endl;
      return;
   }

   NesFileHeader header(nesFile);
   
   ram.loadROM(header.getPRGROM());
   vram.load(header.getCHRROM());
   
   ppu->loadVRAM(vram);
   cpu->loadRAM(&ram);
}

uint32_t* NES::emulateNextFrame()
{
	while(!ppu->isFrameReady()) {
		cpu->execute();
	}
	std::cout << "Frame ptr : " << ppu->getFrame() << std::endl;
	return ppu->getFrame();
}
