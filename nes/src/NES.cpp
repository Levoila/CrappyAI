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
	return ppu->getFrame();
}

void NES::setController1State(uint8_t state)
{
	ram.setController1(state);
}

void NES::saveState(std::string filename)
{
	std::ofstream file(filename.c_str(), std::ios::binary);
	
	cpu->saveState(file);
	ppu->saveState(file);
}

void NES::loadState(std::string filename)
{
	std::ifstream file(filename.c_str(), std::ios::binary);
	
	if (!file) {
		std::cout << "File " << filename << "does not exist." << std::endl;
		return;
	}
	
	cpu->loadState(file);
	ppu->loadState(file);
}
