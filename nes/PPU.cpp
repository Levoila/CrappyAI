#include "PPU.h"

#include <iostream>
#include <iomanip>
#include <bitset>

using namespace std;

PPU::PPU()
   : CTRL(0), MASK(0), STATUS(0), SCROLL(0), ADDR(0), DATA(0),
     OAMADDR(0), OAMDATA(0), cVAddr(0), tVAddr(0), fineXScroll(0), 
     writeToggle(false), scanlineNb(0), pixelNb(0), oddFrame(false),
     NMISignal(false), spriteZeroActive(false),
     frameCount(0), frameReady(false)
{
   rgba[0] = 0x75757500;
   rgba[1] = 0x271B8F00;
   rgba[2] = 0x0000AB00;
   rgba[3] = 0x47009F00;
   rgba[4] = 0x8F007700;
   rgba[5] = 0xAB001300;
   rgba[6] = 0xA7000000;
   rgba[7] = 0x7F0B0000;
   rgba[8] = 0x432F0000;
   rgba[9] = 0x00470000;
   rgba[10] = 0x00510000;
   rgba[11] = 0x003F1700;
   rgba[12] = 0x1B3F5F00;
   rgba[13] = 0x00000000;
   rgba[14] = 0x00000000;
   rgba[15] = 0x00000000;
   rgba[16] = 0xBCBCBC00;
   rgba[17] = 0x0073EF00;
   rgba[18] = 0x233BEF00;
   rgba[19] = 0x8300F300;
   rgba[20] = 0xBF00BF00;
   rgba[21] = 0xE7005B00;
   rgba[22] = 0xDB2B0000;
   rgba[23] = 0xCB4F0F00;
   rgba[24] = 0x8B730000;
   rgba[25] = 0x00870000;
   rgba[26] = 0x00AB0000;
   rgba[27] = 0x00933B00;
   rgba[28] = 0x00838B00;
   rgba[29] = 0x00000000;
   rgba[30] = 0x00000000;
   rgba[31] = 0x00000000;
   rgba[32] = 0xFFFFFF00;
   rgba[33] = 0x3FBFFF00;
   rgba[34] = 0x5F97FF00;
   rgba[35] = 0xA78DFD00;
   rgba[36] = 0xF77BFF00;
   rgba[37] = 0xFF77B700;
   rgba[38] = 0xFF776300;
   rgba[39] = 0xFF9B3B00;
   rgba[40] = 0xF3BF3F00;
   rgba[41] = 0x83D31300;
   rgba[42] = 0x4FDF4B00;
   rgba[43] = 0x58F89800;
   rgba[44] = 0x00EBDB00;
   rgba[45] = 0x00000000;
   rgba[46] = 0x00000000;
   rgba[47] = 0x00000000;
   rgba[48] = 0xFFFFFF00;
   rgba[49] = 0xABE7FF00;
   rgba[50] = 0xC7D7FF00;
   rgba[51] = 0xD7CBFF00;
   rgba[52] = 0xFFC7FF00;
   rgba[53] = 0xFFC7DB00;
   rgba[54] = 0xFFBFB300;
   rgba[55] = 0xFFDBAB00;
   rgba[56] = 0xFFE7A300;
   rgba[57] = 0xE3FFA300;
   rgba[58] = 0xABF3BF00;
   rgba[59] = 0xB3FFCF00;
   rgba[60] = 0x9FFFF300;
   rgba[61] = 0x00000000;
   rgba[62] = 0x00000000;
   rgba[63] = 0x00000000;

   //Oops, Intel
   //cpus are little endian...
   for(int i = 0; i < 64; ++i) {
      uint8_t* bytes = reinterpret_cast<uint8_t*>(&rgba[i]);
      uint8_t temp = bytes[0];
      uint32_t val = (0xFF << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
      rgba[i] = val;
   }
}
void PPU::reset()
{
   CTRL = 0;
   MASK = 0;
   SCROLL = 0;
   DATA = 0;
   cVAddr = 0;
   tVAddr = 0;
   oddFrame = false;
   writeToggle = false;
   scanlineNb = 0;
   pixelNb = 0;
   frameCount = 0;
}


void PPU::render()
{
   // Sprite and background rendering disabled
   if((MASK & 0x18) == 0 && scanlineNb <= 239) {
      return;
   }

   //Visible scanlines
   if(scanlineNb <= 239) {
      if(pixelNb == 0) return;

		//Copy horizontal bits from t to v si visible scanline ou pre render scanline
		if(pixelNb == 257 && MASK & 0x18) {
			cVAddr &= 0xFBE0;
			cVAddr |= (tVAddr & 0x041F);
		}

      if(pixelNb <= 256) {
		  spriteEvaluation();
         //Activate sprites if they need to be rendered
         //and deactivate them when they are finished
         for(uint8_t i = 0; i < 8; ++i) {
            if(xPosCounter[i] == 0) {
               activeSprite[i] = true; 
            }
            if(activeCounter[i] >= 8) {
               activeSprite[i] = false;
            }
            --xPosCounter[i];
            if(activeSprite[i]) {
               ++activeCounter[i];
            }
         }

         frame[scanlineNb*256 + pixelNb - 1] = rgba[pixelFetch()];

         //Shifting registers
         tileBMPShift[0] <<= 1;
         tileBMPShift[1] <<= 1;
         paletteShift[0] <<= 1;
         paletteShift[1] <<= 1;
         for(uint8_t i = 0; i < 8; ++i) {
            if(activeSprite[i]) {
               //Flip horizontally
               if(spriteAttribute[i] & 0x40) {
                  spriteBMPShift[0][i] >>= 1;
                  spriteBMPShift[1][i] >>= 1;
               } else {
                  spriteBMPShift[0][i] <<= 1;
                  spriteBMPShift[1][i] <<= 1;
               }
            }
         }

         if((pixelNb % 8) == 0) {
            tileFetch();
         }

         //Y scroll increment
         if(pixelNb == 256) {
            if((cVAddr & 0x7000) != 0x7000)
               cVAddr += 0x1000;
            else {
               cVAddr &= ~0x7000;
               uint16_t y = (cVAddr & 0x03E0) >> 5;
               if(y == 29) {
                  y = 0;
                  cVAddr ^= 0x0800;
               } else if(y == 31) {
                  y = 0;
               } else {
                  ++y;
               }
               cVAddr = (cVAddr & ~0x3E0) | (y << 5);
            }
         }
      } else if(pixelNb <= 320) {
         spriteEvaluation();
      } else if(pixelNb == 321) {
         tileFetch();

         tileBMPShift[0] <<= 8;
         tileBMPShift[1] <<= 8;
         paletteShift[0] <<= 8;
         paletteShift[1] <<= 8;

         tileFetch();
      }
      //Post-render scanline
   } else if(scanlineNb == 240) {
      //Idle
      return;

      //V-blank scanlines
   } else if(scanlineNb == 241) {
	   //Initialy 1, but a test rom said I clear my vbl flag too late
      if(pixelNb == 1) { 
         STATUS |= 0x80;
         if(CTRL & 0x80) { //NMI enabled
            NMISignal = true;
         }
      }
   } else if(scanlineNb <= 260) {
    
      //Pre-render scanline
   } else if(scanlineNb <= 261) {
	   spriteEvaluation();
      //Clear vBlank, sprite overflow and sprite 0 hit flags
      if(pixelNb == 1) {
         STATUS &= 0x1F;

		 //Copy horizontal bits from t to v si visible scanline ou pre render scanline
      } else if(pixelNb == 257 && (MASK & 0x18)) {
			cVAddr &= 0xFBE0;
			cVAddr |= (tVAddr & 0x041F);

			//Copy vertical bits from t to v
	  }else if(pixelNb == 320 && (MASK & 0x18)) {
         cVAddr &= 0x041F;
         cVAddr |= (tVAddr & 0xFBE0);
      } else if(pixelNb == 321 && (MASK & 0x18)) {
         tileFetch();

         tileBMPShift[0] <<= 8;
         tileBMPShift[1] <<= 8;
         paletteShift[0] <<= 8;
         paletteShift[1] <<= 8;

         tileFetch();

      //Skip last tick on oddFrame if rendering (BG and/or sprite enabled)
      } else if(pixelNb == 339 && oddFrame && (MASK & 0x18) > 0) {
		  ++pixelNb;
      }
   }
}

void PPU::tick()
{

   render();

   if(pixelNb == 340) {
      pixelNb = 0;
      ++scanlineNb;
      if(scanlineNb == 262) {
         scanlineNb = 0;
         ++frameCount;
         oddFrame = !oddFrame;

         //Render actual pixels on screen
         renderWindow();
      }
   } else {
      ++pixelNb;
   }
}

uint8_t PPU::pixelFetch()
{
   //Palette index for bg and sprite pixel
   uint16_t pixelMask = 0x8000 >> fineXScroll;
   uint8_t bgIndex = ((tileBMPShift[0] & pixelMask) ? 0x01 : 0x00) |
                     ((tileBMPShift[1] & pixelMask) ? 0x02 : 0x00);
   uint8_t bgPaletteIndex = ((paletteShift[0] & pixelMask) ? 0x01 : 0x00) |
                            ((paletteShift[1] & pixelMask) ? 0x02 : 0x00);

   uint8_t spriteIndex = 0;

   uint8_t i = 0;
   for(; i < 8; ++i) {
      if(activeSprite[i]) {
         //flip horizontally or not
         pixelMask = (spriteAttribute[i] & 0x40) ? 0x01 : 0x80;
         spriteIndex = ((spriteBMPShift[0][i] & pixelMask) ? 0x01 : 0x00) |
                       ((spriteBMPShift[1][i] & pixelMask) ? 0x02 : 0x00);
         
      }
      
      if(spriteIndex != 0) {
         break;
      }
   }

   //Hide  background
   if(!(MASK & 0x08)) {
      bgIndex = 0;
   }
   //Hide sprites 
   if(!(MASK & 0x10)) {
      spriteIndex = 0;
   }

   //0 sprite hit
   if(spriteZeroActive && activeSprite[0] && pixelNb >= ((MASK & 0x06) != 0x06 ? 8 : 0) && pixelNb < 255) {
	   if(bgIndex != 0 && spriteIndex != 0) { //Hit !
         STATUS |= 0x40;
      }
   }

   // 0 : behind bg  1 : front
   bool spritePriority = ((spriteAttribute[i] & 0x20) ? false : true);

   uint16_t bgColorAddr = 0x3F00 | ((bgIndex) | (bgPaletteIndex << 2));
   uint16_t spriteColorAddr = 0x3F10 | ((spriteIndex) | ((spriteAttribute[i] & 0x03) << 2));

   uint8_t bgColor = ram.read(bgColorAddr) & 0x3F; //Sometimes the value is >= 0x40 for some reason...
   uint8_t spriteColor = ram.read(spriteColorAddr) & 0x3F;
   uint8_t defaultBgColor = ram.read(0x3F00) & 0x3F;

   //Rendering disabled in leftmost 8 pixels (	JAMAIS TESTÉ!!!!	)
   if(pixelNb < 8 && (MASK & 0x02) == 0) {
	   bgColor = defaultBgColor;
   }
   if(pixelNb < 8 && (MASK & 0x04) == 0) {
	   spriteColor = defaultBgColor;
   }

   if(bgIndex == 0) {
      if(spriteIndex == 0) {
         return defaultBgColor;
      } else {
         return spriteColor;
      }
   } else {
      if(spriteIndex == 0) {
         return bgColor;
      } else {
         if(spritePriority) {
            return spriteColor;
         } else {
            return bgColor;
         }
      }
   }
}

void PPU::tileFetch()
{
   uint16_t tileAddr = 0x2000 | (cVAddr & 0x0FFF);
   uint16_t attributeAddr = 0x23C0 | (cVAddr & 0x0C00) | ((cVAddr >> 4) & 0x38) | ((cVAddr >> 2) & 0x07);

   //Tile bitmap fetch
   uint8_t bmpIndex = ram.read(tileAddr);

   uint16_t baseBmpAddr = (CTRL & 0x10) ? 0x1000 : 0x0000;
   //                                                       fine y scroll
   uint16_t bmp1 = ram.read(baseBmpAddr + bmpIndex * 0x10 + (cVAddr >> 12));
   uint16_t bmp2 = ram.read(baseBmpAddr + bmpIndex * 0x10 + 8 + (cVAddr >> 12));
   tileBMPShift[0] &= 0xFF00;
   tileBMPShift[1] &= 0xFF00;
   tileBMPShift[0] |= bmp1;
   tileBMPShift[1] |= bmp2;

   //Tile attribute fetch
   uint8_t attribute = ram.read(attributeAddr);
   uint8_t palette = 0;
   uint8_t coarseX = cVAddr & 0x001F;
   uint8_t coarseY = ((cVAddr & 0x03E0) >> 5);

   if(coarseX % 4 > 1) {
      if(coarseY % 4 > 1) {
         palette = ((attribute & 0xC0) >> 6);
      } else {
         palette = ((attribute & 0x0C) >> 2);
      }
   } else {
      if(coarseY % 4 > 1) {
         palette = ((attribute & 0x30) >> 4);
      } else {
         palette = (attribute & 0x03);
      }
   }

   uint16_t bit0 = ((palette & 0x01) ? 0x00FF : 0x0000);
   uint16_t bit1 = ((palette & 0x02) ? 0x00FF : 0x0000);

   paletteShift[0] &= 0xFF00;
   paletteShift[1] &= 0xFF00;
   paletteShift[0] |= bit0;
   paletteShift[1] |= bit1;

   //Coarse X scroll increment
   if((cVAddr & 0x001F) == 31) {
      cVAddr &= ~0x001F;
      cVAddr ^= 0x400;
   } else {
      ++cVAddr;
   }
   
}

void PPU::spriteEvaluation()
{
   //Clear secondary OAM
   if(pixelNb == 1) {
      for(unsigned int i = 0; i < 32; ++i) {
         secondaryOAM[i] = 0xFF;
      }
      //Sprite evaluation
   } else if(pixelNb == 64) {
	   spriteZeroActiveNextScanline = false;
      uint8_t secondaryOAMIndex = 0;
      uint8_t spriteSize = ((CTRL & 0x20) ? 16 : 8);
      for(unsigned int i = 0; i < 64; ++i) {
         uint8_t y = primaryOAM[i*4];
         if(secondaryOAMIndex != 8)
            secondaryOAM[secondaryOAMIndex*4] = y; //Even if not in range
         //If sprite in range
         if(y <= scanlineNb && (y + spriteSize) >  scanlineNb) {
            if(secondaryOAMIndex == 8) {
               //STATUS |= 0x20; //Sprite overflow
               // TODO : IMPLEMENT SPRITE OVERFLOW BUG
               return;
            }
            for(unsigned int j = 1; j < 4; ++j) {
               secondaryOAM[secondaryOAMIndex*4 + j] = primaryOAM[i*4 + j];
            }
            ++secondaryOAMIndex;

            if(i == 0) {
               spriteZeroActiveNextScanline = true;
            }
         }
	  }
     // Sprite fetches
   } else if(pixelNb == 257) {
	   spriteZeroActive = spriteZeroActiveNextScanline;
      for(unsigned int i = 0; i < 8; ++i) {
         uint8_t y = secondaryOAM[i*4];
         if(y >= 0xEF) {
            spriteBMPShift[0][i] = 0;
            spriteBMPShift[1][i] = 0;
            continue;
         }
         uint8_t tileIndex = secondaryOAM[i*4+1];
         
		 uint8_t line = 0;
		 if(CTRL & 0x20) { //8x16
			 line = (scanlineNb - y) & 0xF;
		 } else {
			 line = (scanlineNb - y) & 0x7;
		 }

         spriteAttribute[i] = secondaryOAM[i*4 + 2];
         xPosCounter[i] = secondaryOAM[i*4 + 3];
         activeSprite[i] = false;
         activeCounter[i] = 0;
         
         //Flip vertically
         if(spriteAttribute[i] & 0x80) {
			 if(CTRL & 0x20) {
				 line = 15 - line;
			 } else {
				line = 7 - line;
			 }
         }

         // 8x16 sprites
         if(CTRL & 0x20) {
            uint16_t baseAddr = ((tileIndex & 0x01) ? 0x1000 : 0x0000);
            uint8_t nbSprite = line / 8;
            line %= 8;
            spriteBMPShift[0][i] = ram.read(baseAddr + (tileIndex >> 1) * 32 + line + nbSprite * 16);
            spriteBMPShift[1][i] = ram.read(baseAddr + (tileIndex >> 1) * 32 + line + nbSprite * 16 + 8);
            // 8x8 sprites
         } else {
            uint16_t baseAddr = ((CTRL & 0x08) ? 0x1000 : 0x0000);
            spriteBMPShift[0][i] = ram.read(baseAddr + tileIndex * 16 + line);
            spriteBMPShift[1][i] = ram.read(baseAddr + tileIndex * 16 + line + 8);
         }
      }
   }
}

uint8_t PPU::ctrl(uint8_t d, bool w)
{
   if(w) {
      //Toggling NMI_enable bit from 0 to 1 when NMI_occurred bit is set causes
      //an NMI
      if((d & 0x80) && (CTRL & 0x80) == 0 && (STATUS & 0x80)) {
         NMISignal = true;
      }
      CTRL = d;

      //Put nametable select into Temporary VRAM address
      uint16_t temp = d;
	  temp <<= 10;
	  temp &= 0x0C00;

      tVAddr &= 0xF3FF;
      tVAddr |= temp;
   } else {
      
   }
   return CTRL;
}

uint8_t PPU::mask(uint8_t d, bool w) 
{
   if(w) {
      MASK = d;
   } else {

   }
   return MASK;
}

uint8_t PPU::status(uint8_t d, bool w)
{
   if(w) {
	  STATUS &= 0x80; //Clear everything but VBL flag
      STATUS |= d & 0x7F; // Writing $2002 shouldn't affect VBL flag
   } else {
      d = STATUS;
	  //Clear write toggle
	  writeToggle = false;
	  //Clear vBlank bit
      STATUS &= 0x7F;
   }
   return d;
}

uint8_t PPU::scroll(uint8_t d, bool w)
{
   if(w) {
      SCROLL = d ;
      if(!writeToggle) { //First write
         tVAddr &= 0xFFE0;
         tVAddr |= (static_cast<uint16_t>(SCROLL) >> 3);
         fineXScroll = SCROLL & 0x07;
      } else {
         uint16_t fineY = (static_cast<uint16_t>(SCROLL) & 0x0007) << 12;
         uint16_t coarseY = (static_cast<uint16_t>(SCROLL) & 0x00F8) << 2;
         tVAddr &= 0x0C1F;
         tVAddr |= fineY | coarseY;
      }
      writeToggle = !writeToggle;
   } else {

   }
   return SCROLL;
}

uint8_t PPU::addr(uint8_t d, bool w)
{
   if(w) {
      ADDR = d;
      if(!writeToggle) { //First write
         tVAddr &= 0x00FF;
         tVAddr |= (static_cast<uint16_t>(ADDR) & 0x003F)  << 8;
      } else {
         tVAddr &= 0xFF00;
         tVAddr |= static_cast<uint16_t>(ADDR);
         cVAddr = tVAddr;
      }
      writeToggle = !writeToggle;
   } else {

   }
   return ADDR;
}

uint8_t PPU::data(uint8_t d, bool w)
{
	uint16_t addr = cVAddr & 0x3FFF;

   if(w) {
      DATA = d;
      ram.write(addr, DATA);
   } else { 
	   DATA = DATA_READ_BUFFER;
      DATA_READ_BUFFER = ram.read(addr);
	  if(addr > 0x3EFF) { //Si on lit la palette, on load direct les données et le buffer est chargé avec la nametable
							//correspondante
		  DATA = DATA_READ_BUFFER;
		  DATA_READ_BUFFER = ram.read(addr - 0x1000);
	  }
   } 
   uint8_t increment = ((CTRL & 0x04) ? 32 : 1);
   cVAddr = ( cVAddr + increment) & 0x3FFF;
   return DATA;
}

uint8_t PPU::oamAddr(uint8_t d, bool w)
{
   if(w) {
      OAMADDR = d;
   } else {

   }
   return OAMADDR;
}

uint8_t PPU::oamData(uint8_t d, bool w)
{
   if(w) {
      OAMDATA = d;
      primaryOAM[OAMADDR] = OAMDATA;
      ++OAMADDR;
   } else {
      OAMDATA = primaryOAM[OAMADDR];
   }
   return OAMDATA;
}

bool PPU::NMISignalOn() {
   bool signal = NMISignal;
   if(signal) {
      NMISignal = false;
   }
   return signal;
}

bool PPU::isFrameReady()
{
   bool ready = frameReady;
   if(frameReady)
      frameReady = false;
   return ready;
}

void PPU::renderWindow()
{
   /*sf::Image img;
   img.create(256, 240, (sf::Uint8*)frame);
   sf::Texture tex;
   tex.loadFromImage(img);
   sf::Sprite sprite;
   sprite.setTexture(tex);
   sprite.setScale(4, 3.5);
   window.draw(sprite);*/
   frameReady = true; 
}

void PPU::dumpAttributeTables(std::ostream& os) const
{
	uint16_t attrAddr = 0x23C0;
	uint16_t size = 64;
	uint8_t dump[16][16];

	for(unsigned int k = 0; k < 4; ++k) {
		for(unsigned int i = 0; i < size; ++i) {
			uint8_t square = ram.read(attrAddr + i);

			int x = i % 8;
			int y = i / 8;

			dump[2*x][2*y] = square & 0x03;
			dump[2*x+1][2*y] = (square >> 2) & 0x03;
			dump[2*x][2*y+1] = (square >> 4) & 0x03;
			dump[2*x+1][2*y+1] = (square >> 6) & 0x03;
		}

		os << "##########   Attribute table : " << (k+1) << "   ##########" << std::endl << std::endl;

		for(unsigned int i = 0; i < 16; ++i) {
			os << "  ";
			for(unsigned int j = 0; j < 15; ++j) {
				char c;
				switch(dump[j][i]) {
				case 0:
					c = '*';
					break;
				case 1:
					c = '#';
					break;
				case 2:
					c = '$';
					break;
				case 3:
					c = '&';
					break;
				}
				os << c<< "  ";
			}
			os << std::endl;
		}
		os << "\n\n\n";

		attrAddr += 0x400;
	}
}

void PPU::dumpNametables(std::ostream& os) const
{
	uint16_t nAddr = 0x2000;

	for(unsigned int k = 0; k < 4; ++k) {
		os << "##########   Nametable : " << (k+1) << "   ##########\n\n";

		for(unsigned int i = 0; i < 30; ++i) {
			os << "  ";
			for(unsigned int j = 0; j < 32; ++j) {
				os.width(4);
				os << (unsigned int)ram.read(nAddr + i * 32 + j);
			}
			os << std::endl;
		}

		nAddr += 0x400;
	}
}

void PPU::dumpPatternTables(std::ostream& os) const
{
	uint8_t p[2][256][8][8];

	for(unsigned int k = 0; k < 2; ++k) {
		for(unsigned int i = 0; i < 256; ++i) {
			for(unsigned int j = 0; j < 8; ++j) {
				uint8_t b1 = ram.read(0x1000*k + i*16 + j);
				uint8_t b2 = ram.read(0x1000*k + i*16 + j + 8);
				for(unsigned int l = 0; l < 8; ++l) {
					p[k][i][j][l] = ((b1 >> (7-l)) & 0x01) + (((b2 >> (7-l) & 0x01)) << 1);
				}
			}
		}
	}

	os << "  Pattern table #1       Pattern table #2\n\n";

	for(unsigned int i = 0; i < 256; ++i) {
		os << "      Tile #" << i << "                Tile #" << i << std::endl;
		for(unsigned int j = 0; j < 8; ++j) {
			os << "  ";
			for(unsigned int k = 0; k < 8; ++k) {
				os << (unsigned int)p[0][i][j][k] << " ";
			}
			os << "       ";
			for(unsigned int k = 0; k < 8; ++k) {
				os << (unsigned int)p[1][i][j][k] << " ";
			}
			os << std::endl;
		}
		os << std::endl << std::endl;
	}

	for(unsigned int i = 0; i < 0x10000; ++i) {
		os << std::hex << "0x" << std::setw(6) << (i & 0x3fff) << std::dec << " (" << std::setw(6) << i << ")  : " << (unsigned int)ram.read(i) << std::endl;
	}
}















