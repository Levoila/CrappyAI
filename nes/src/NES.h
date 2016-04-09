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
#include <vector>

class NES {
public:
	NES();
	
	void loadROM(std::string filename);
	uint32_t* emulateNextFrame();
	void setController1State(uint8_t state);
	void saveState(std::string filename);
	void loadState(std::string filename);
	std::vector<uint16_t> findValue(uint8_t value);
	std::vector<uint16_t> findValue(uint16_t value);
	uint8_t readMem(uint16_t addr);
private:
	PPU* ppu;
	CPU* cpu;
	VRAM vram;
	CPURam ram;
};

#endif
