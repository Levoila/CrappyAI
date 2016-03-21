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
}
