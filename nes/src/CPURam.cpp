#include "CPURam.h"
#include <string.h>
#include <iostream>
#include <fstream>

using namespace std;

CPURam::CPURam(CPU* cp, PPU* pp)
	:cpu(cp), ppu(pp), controllerStrobe(false)
{
   for(unsigned int i = 0; i < 0x10000; ++i) {
      mem[i] = 0xFF;
   }
   mem[0xf] = 0xBF; //wtf??? bump n ride lit de 0xf alors qu'il n'es pas initialis� et met �a dans 0x2000
   mem[0xa] = 0xDF;
   mem[0x9] = 0xEF;
   mem[0x8] = 0xF7;
}

void CPURam::loadROM(vector<uint8_t*> rom) {
   memcpy(mem+0x8000, rom[0], 0x4000);
   if(rom.size() > 1) {
      memcpy(mem+0xC000, rom[1], 0x4000);
   } else {
      memcpy(mem+0xC000, rom[0], 0x4000);
   }
}

void CPURam::saveState(std::ostream& os)
{
	os.write((char*)(&controller1), sizeof(controller1));
	os.write((char*)(&controllerStrobe), sizeof(controllerStrobe));
	os.write((char*)mem, sizeof(char)*0x10001);
}

void CPURam::loadState(std::istream& os)
{
	os.read((char*)(&controller1), sizeof(controller1));
	os.read((char*)(&controllerStrobe), sizeof(controllerStrobe));
	os.read((char*)mem, sizeof(char)*0x10001);
}

void CPURam::write(uint16_t address, uint8_t data)
{
   //cout << "Writing to " << hex << (unsigned int)address << " : " << (unsigned int)data << dec << endl;
   mem[decode(address, true, data)] = data;
}

uint8_t CPURam::read(uint16_t address)
{
   //cout << "Reading address " << hex << (unsigned int)address << " : ";
   uint8_t val = mem[decode(address)];
   //cout << (unsigned int)val << dec << endl;
   return val;
}

std::vector<uint16_t> CPURam::findValue(uint8_t value)
{
	std::vector<uint16_t> addresses;
	for (unsigned int i = 0; i < 0x800; ++i) {
		if (mem[i] == value) {
			addresses.push_back(i);
		}
	}
	
	for (auto addr : addresses) {
		std::cout << std::hex << "Addr : " << addr << std::endl;
	}
	
	return addresses;
}

std::vector<uint16_t> CPURam::findValue(uint16_t value)
{
	std::vector<uint16_t> addresses;
	for (unsigned int i = 0; i < 0x7FF; ++i) {
		uint16_t memValue = mem[i] | (mem[i+1] << 8);
		if (memValue == value) {
			addresses.push_back(i);
		}
	}
	
	for (auto addr : addresses) {
		std::cout << std::hex << "Addr : " << addr << std::endl;
	}
	
	return addresses;
}

uint32_t CPURam::decode(uint16_t addr, bool w, uint8_t d)
{
   if(addr < 0x2000) {
      addr &= 0x07FF;
   } else if(addr < 0x4000) {
      addr &= 0x2007;

      switch(addr) {
         case 0x2000:
            mem[0x2000] = ppu->ctrl(d,w);
            break;
         case 0x2001:
            mem[0x2001] = ppu->mask(d,w);
            break;
         case 0x2002:
            mem[0x2002] = ppu->status(d,w);
            break;
         case 0x2003:
            mem[0x2003] = ppu->oamAddr(d,w);
            break;
         case 0x2004:
            mem[0x2004] = ppu->oamData(d,w);
            break;
         case 0x2005:
            mem[0x2005] = ppu->scroll(d,w);
            break;
         case 0x2006:
            mem[0x2006] = ppu->addr(d,w);;
            break;
         case 0x2007:
            mem[0x2007] = ppu->data(d,w);
            break;
         default:
            break;
      }
   } else if(addr < 0x4020) {
      switch(addr) {
         case 0x4014: //OAM_DMA
            if(w) {
               uint16_t addr = 0x100 * d;
               cpu->tick();
               //It should tick an other time if it's an odd cpu cycle
               for(unsigned int i = 0; i < 256; ++i) {
                  uint8_t data = mem[addr+i];
                  cpu->tick();
                  mem[0x2004] = ppu->oamData(data, true);
                  cpu->tick();
               }
            }
            break;
		case 0x4016:
			if(w) {
				if(d & 0x1) {
					controllerStrobe = true;
					controller1 = controller1State;
					mem[0x4016] = controller1 & 0x01;
					mem[0x4017] = (mem[0x4017] & 0xC0) | (controller1 & 0x01);
				} else {
					controllerStrobe = false;
				}
				return 0x10000;
			} else {
				if(!controllerStrobe) {
					mem[0x4016] = (controller1 & 0x01);
					mem[0x4017] = (mem[0x4017] & 0xC0) | (controller1 & 0x01);
					controller1 >>= 1;
					controller1 |= 0x80;
				}
			}
			break;
         default:
            break;
      }
   } else if(addr >= 0x8000 && w) { //Mapper 03
	   uint8_t CHR = d & 0x03;
	   uint16_t offset = (CHR == 0 ? 0x0000 : 0x2000) + CHR * 0x2000;
	   ppu->ram.CHROffset = offset;
	   //std::cout << hex << (unsigned int) d << std::endl;
	   return 0x10000;
   }

   return addr;
}
