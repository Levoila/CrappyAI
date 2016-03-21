#ifndef NES_H
#define NES_H

#include "CPU.h"
#include "PPU.h"
#include "VRAM.h"
#include "CPURam.h"
#include "NesFileHeader.h"

#include <stdint.h>
#include <string>
#include <chrono>

class NES {
public:
	NES();
	
	void loadROM(std::string filename);
	uint32_t* emulateNextFrame();
private:
	PPU* ppu;
	CPU* cpu;
	VRAM vram;
	CPURam ram;
};

#endif
