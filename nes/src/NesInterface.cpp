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
}
