#include "NES.h"

extern "C" 
{
	NES* createEmulator()
	{
		return new NES();
	}
	
	uint32_t* emulateNextFrame(NES* nes)
	{
		return nes->emulateNextFrame();
	}
	
	void loadROM(NES* nes, char* filename)
	{
		nes->loadROM(filename);
	}
	
	void setController1State(NES* nes, uint8_t state)
	{
		nes->setController1State(state);
	}
	
	void saveState(NES* nes, char* filename)
	{
		nes->saveState(filename);
	}
	
	void loadState(NES* nes, char* filename)
	{
		nes->loadState(filename);
	}
	
	void findValue8(NES* nes, uint8_t value)
	{
		nes->findValue(value);
	}
	
	void findValue16(NES* nes, uint16_t value)
	{
		nes->findValue(value);
	}
	
	uint8_t readMem8(NES* nes, uint16_t addr)
	{
		return nes->readMem(addr);
	}
	
	uint16_t readMem16(NES* nes, uint16_t addr)
	{
		return nes->readMem(addr) | (nes->readMem(addr+1) << 8);
	}
}
