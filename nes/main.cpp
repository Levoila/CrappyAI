#include <iostream>
#include <chrono>

#include "NES.h"

using namespace std;

int main(int argc, char** argv)
{
	NES nes;
	
	nes.loadROM("smb.nes");
	
	while (true) {
		uint32_t* frame = nes.emulateNextFrame();
	}
}
