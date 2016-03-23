#include "NesFileHeader.h"
#include <iostream>

using namespace std;

std::istream& operator>>(std::istream& is, Mirroring& m)
{
	int val;
	
	if (is >> val) {
		switch(val) {
			case VERTICAL:
			case HORIZONTAL:
			case FOUR_SCREEN:
			case SINGLE_SCREEN:
				m = Mirroring(val);
				break;
		}
	}
	
	return is;
}

NesFileHeader::NesFileHeader(ifstream& file)
{
   char NES[4];
   file.read(NES, 4);
   assert(NES[0] == 'N' && NES[1] == 'E' && NES[2] == 'S' && NES[3] == 0x1A);

   uint8_t nbPRGROM = 0;
   file.read((char*)&nbPRGROM, 1);

   uint8_t nbCHRROM = 0;
   file.read((char*)&nbCHRROM, 1);

   uint8_t flag6 = 0;
   file.read((char*)&flag6, 1);
   mapper = flag6 >> 4;
   if(flag6 & 0x01) {
      mirroring = Mirroring::VERTICAL;
   } else {
      mirroring = Mirroring::HORIZONTAL;
   }
   if(flag6 & 0x08) {
      mirroring = Mirroring::FOUR_SCREEN;
   }

   assert(!(flag6 & 0x04)); //No trainer support (yet?)
   batteryBackedSRAM = flag6 & 0x02;

   uint8_t flag7 = 0;
   file.read((char*) &flag7, 1);
   mapper |= flag7 & 0xF0;
   assert(!(flag7 & 0x03)); //No support for playchoice and Vs. Unisystem

   file.seekg(8, file.cur);

   for(unsigned int i = 0; i < nbPRGROM; ++i) {
      uint8_t* rom = new uint8_t[0x4000];
      file.read((char*) rom, 0x4000);
      PRGROM.push_back(rom);
   }

   for(unsigned int i = 0; i < nbCHRROM; ++i) {
      uint8_t* rom = new uint8_t[0x2000];
      file.read((char*) rom, 0x2000);
      CHRROM.push_back(rom);
   }
   cout << "nbPRGROM : " << (unsigned int)nbPRGROM << endl
      << "nbCHRROM : " << (unsigned int)nbCHRROM << endl
	  << "Mapper : " << (unsigned int)mapper << endl
	  << "Mirroring : " << mirroring << endl;
}
