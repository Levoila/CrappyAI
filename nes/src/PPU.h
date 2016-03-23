#ifndef PPU_H
#define PPU_H

#include <stdint.h>
#include <vector>
#include <ostream>

#include "VRAM.h"

class CPURam;

class PPU
{
   public:
      PPU();

      void tick();
      uint8_t ctrl(uint8_t d = 0, bool w = false); //Data and write
      uint8_t mask(uint8_t d = 0, bool w = false);
      uint8_t status(uint8_t d = 0, bool w = false);
      uint8_t scroll(uint8_t d = 0, bool w = false);
      uint8_t addr(uint8_t d = 0, bool w = false);
      uint8_t data(uint8_t d = 0, bool w = false);
      uint8_t oamAddr(uint8_t d = 0, bool w = false);
      uint8_t oamData(uint8_t d = 0, bool w = false);
      bool NMISignalOn();
      void reset();
      void loadVRAM(VRAM& vram) { ram = vram; }
      bool isFrameReady();
      void renderWindow();
      uint32_t* getFrame() { return frame; }
      void saveState(std::ostream& os);
      void loadState(std::istream& os);

	  void dumpAttributeTables(std::ostream& os) const;
	  void dumpNametables(std::ostream& os) const;
	  void dumpPatternTables(std::ostream& os) const;

	  VRAM ram; //Public to be accessed by CpuRam directly to test some mappers
   private:
      void render();
      void spriteEvaluation();
      void tileFetch();
      uint8_t pixelFetch();


      //Registers (accessible from CPU)
      uint8_t CTRL, MASK, STATUS, SCROLL, ADDR, DATA, OAMADDR, OAMDATA;
	  uint8_t DATA_READ_BUFFER;


      uint16_t cVAddr; //Current VRAM address (15bits)
      uint16_t tVAddr; //Temporary VRAM address (15bits)
      uint8_t fineXScroll; //(3bits)
      bool writeToggle; // first/second write toggle, starts false

      uint16_t tileBMPShift[2]; //Contains bitmap data for 2 tiles
      uint16_t paletteShift[2]; //Contains palette data for lower 8pixels of tileBMPShift

      uint8_t primaryOAM[256]; //Primary Object Attribute Memory
      uint8_t secondaryOAM[32]; //Secondary Object Attribute Memory
      uint8_t spriteBMPShift[2][8]; //Contains bitmap data for up to 8 sprites
      uint8_t spriteAttribute[8]; // Contains attributes for up to 8 sprites
      uint8_t xPosCounter[8]; //Contains x position of up to 8 sprites
      bool activeSprite[8];
      uint8_t activeCounter[8]; //Count how many cycles each sprite have been active
      bool spriteZeroActive;
	  bool spriteZeroActiveNextScanline;

      uint16_t scanlineNb; //Index of the current scanline
      uint16_t pixelNb; //Index of the current pixel being rendered

      bool oddFrame;

      bool NMISignal;

      uint32_t frame[240*256];
      uint32_t rgba[0x40];
      uint32_t frameCount;
      bool frameReady;
};

#endif
