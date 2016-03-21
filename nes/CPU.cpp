#include "CPU.h"

#include <iostream>
#include <iomanip>

using namespace std;

CPU::CPU(PPU* p)
   : S(0xFD), PC(0xC000), X(0), Y(0), A(0), resetSignal(true), ppu(p), cycleCount(0)
{

   P.raw = 0x24;
   uPrograms[0x0] = &CPU::BRKImplicit;
   uPrograms[0x1] = &CPU::ORAIndexedIndirect;
   uPrograms[0x2] = nullptr;
   uPrograms[0x3] = nullptr;
   uPrograms[0x4] = nullptr;
   uPrograms[0x5] = &CPU::ORAZeroPage;
   uPrograms[0x6] = &CPU::ASLZeroPage;
   uPrograms[0x7] = nullptr;
   uPrograms[0x8] = &CPU::PHPImplicit;
   uPrograms[0x9] = &CPU::ORAImmediate;
   uPrograms[0xa] = &CPU::ASLAccumulator;
   uPrograms[0xb] = nullptr;
   uPrograms[0xc] = nullptr;
   uPrograms[0xd] = &CPU::ORAAbsolute;
   uPrograms[0xe] = &CPU::ASLAbsolute;
   uPrograms[0xf] = nullptr;
   uPrograms[0x10] = &CPU::BPLRelative;
   uPrograms[0x11] = &CPU::ORAIndirectIndexed;
   uPrograms[0x12] = nullptr;
   uPrograms[0x13] = nullptr;
   uPrograms[0x14] = nullptr;
   uPrograms[0x15] = &CPU::ORAZeroPageX;
   uPrograms[0x16] = &CPU::ASLZeroPageX;
   uPrograms[0x17] = nullptr;
   uPrograms[0x18] = &CPU::CLCImplicit;
   uPrograms[0x19] = &CPU::ORAAbsoluteY;
   uPrograms[0x1a] = nullptr;
   uPrograms[0x1b] = nullptr;
   uPrograms[0x1c] = nullptr;
   uPrograms[0x1d] = &CPU::ORAAbsoluteX;
   uPrograms[0x1e] = &CPU::ASLAbsoluteX;
   uPrograms[0x1f] = nullptr;
   uPrograms[0x20] = &CPU::JSRAbsolute;
   uPrograms[0x21] = &CPU::ANDIndexedIndirect;
   uPrograms[0x22] = nullptr;
   uPrograms[0x23] = nullptr;
   uPrograms[0x24] = &CPU::BITZeroPage;
   uPrograms[0x25] = &CPU::ANDZeroPage;
   uPrograms[0x26] = &CPU::ROLZeroPage;
   uPrograms[0x27] = nullptr;
   uPrograms[0x28] = &CPU::PLPImplicit;
   uPrograms[0x29] = &CPU::ANDImmediate;
   uPrograms[0x2a] = &CPU::ROLImplicit;
   uPrograms[0x2b] = nullptr;
   uPrograms[0x2c] = &CPU::BITAbsolute;
   uPrograms[0x2d] = &CPU::ANDAbsolute;
   uPrograms[0x2e] = &CPU::ROLAbsolute;
   uPrograms[0x2f] = nullptr;
   uPrograms[0x30] = &CPU::BMIRelative;
   uPrograms[0x31] = &CPU::ANDIndirectIndexed;
   uPrograms[0x32] = nullptr;
   uPrograms[0x33] = nullptr;
   uPrograms[0x34] = nullptr;
   uPrograms[0x35] = &CPU::ANDZeroPageX;
   uPrograms[0x36] = &CPU::ROLZeroPageX;
   uPrograms[0x37] = nullptr;
   uPrograms[0x38] = &CPU::SECImplicit;
   uPrograms[0x39] = &CPU::ANDAbsoluteY;
   uPrograms[0x3a] = nullptr;
   uPrograms[0x3b] = nullptr;
   uPrograms[0x3c] = nullptr;
   uPrograms[0x3d] = &CPU::ANDAbsoluteX;
   uPrograms[0x3e] = &CPU::ROLAbsoluteX;
   uPrograms[0x3f] = nullptr;
   uPrograms[0x40] = &CPU::RTIImplicit;
   uPrograms[0x41] = &CPU::EORIndexedIndirect;
   uPrograms[0x42] = nullptr;
   uPrograms[0x43] = nullptr;
   uPrograms[0x44] = nullptr;
   uPrograms[0x45] = &CPU::EORZeroPage;
   uPrograms[0x46] = &CPU::LSRZeroPage;
   uPrograms[0x47] = nullptr;
   uPrograms[0x48] = &CPU::PHAImplicit;
   uPrograms[0x49] = &CPU::EORImmediate;
   uPrograms[0x4a] = &CPU::LSRAccumulator;
   uPrograms[0x4b] = nullptr;
   uPrograms[0x4c] = &CPU::JMPAbsolute;
   uPrograms[0x4d] = &CPU::EORAbsolute;
   uPrograms[0x4e] = &CPU::LSRAbsolute;
   uPrograms[0x4f] = nullptr;
   uPrograms[0x50] = &CPU::BVCRelative;
   uPrograms[0x51] = &CPU::EORIndirectIndexed;
   uPrograms[0x52] = nullptr;
   uPrograms[0x53] = nullptr;
   uPrograms[0x54] = nullptr;
   uPrograms[0x55] = &CPU::EORZeroPageX;
   uPrograms[0x56] = &CPU::LSRZeroPageX;
   uPrograms[0x57] = nullptr;
   uPrograms[0x58] = &CPU::CLIImplicit;
   uPrograms[0x59] = &CPU::EORAbsoluteY;
   uPrograms[0x5a] = nullptr;
   uPrograms[0x5b] = nullptr;
   uPrograms[0x5c] = nullptr;
   uPrograms[0x5d] = &CPU::EORAbsoluteX;
   uPrograms[0x5e] = &CPU::LSRAbsoluteX;
   uPrograms[0x5f] = nullptr;
   uPrograms[0x60] = &CPU::RTSImplicit;
   uPrograms[0x61] = &CPU::ADCIndexedIndirect;
   uPrograms[0x62] = nullptr;
   uPrograms[0x63] = nullptr;
   uPrograms[0x64] = nullptr;
   uPrograms[0x65] = &CPU::ADCZeroPage;
   uPrograms[0x66] = &CPU::RORZeroPage;
   uPrograms[0x67] = nullptr;
   uPrograms[0x68] = &CPU::PLAImplicit;
   uPrograms[0x69] = &CPU::ADCImmediate;
   uPrograms[0x6a] = &CPU::RORImplicit;
   uPrograms[0x6b] = nullptr;
   uPrograms[0x6c] = &CPU::JMPIndirect;
   uPrograms[0x6d] = &CPU::ADCAbsolute;
   uPrograms[0x6e] = &CPU::RORAbsolute;
   uPrograms[0x6f] = nullptr;
   uPrograms[0x70] = &CPU::BVSRelative;
   uPrograms[0x71] = &CPU::ADCIndirectIndexed;
   uPrograms[0x72] = nullptr;
   uPrograms[0x73] = nullptr;
   uPrograms[0x74] = nullptr;
   uPrograms[0x75] = &CPU::ADCZeroPageX;
   uPrograms[0x76] = &CPU::RORZeroPageX;
   uPrograms[0x77] = nullptr;
   uPrograms[0x78] = &CPU::SEIImplicit;
   uPrograms[0x79] = &CPU::ADCAbsoluteY;
   uPrograms[0x7a] = nullptr;
   uPrograms[0x7b] = nullptr;
   uPrograms[0x7c] = nullptr;
   uPrograms[0x7d] = &CPU::ADCAbsoluteX;
   uPrograms[0x7e] = &CPU::RORAbsoluteX;
   uPrograms[0x7f] = nullptr;
   uPrograms[0x80] = nullptr;
   uPrograms[0x81] = &CPU::STAIndexedIndirect;
   uPrograms[0x82] = nullptr;
   uPrograms[0x83] = nullptr;
   uPrograms[0x84] = &CPU::STYZeroPage;
   uPrograms[0x85] = &CPU::STAZeroPage;
   uPrograms[0x86] = &CPU::STXZeroPage;
   uPrograms[0x87] = nullptr;
   uPrograms[0x88] = &CPU::DEYImplicit;
   uPrograms[0x89] = nullptr;
   uPrograms[0x8a] = &CPU::TXAImplicit;
   uPrograms[0x8b] = nullptr;
   uPrograms[0x8c] = &CPU::STYAbsolute;
   uPrograms[0x8d] = &CPU::STAAbsolute;
   uPrograms[0x8e] = &CPU::STXAbsolute;
   uPrograms[0x8f] = nullptr;
   uPrograms[0x90] = &CPU::BCCRelative;
   uPrograms[0x91] = &CPU::STAIndirectIndexed;
   uPrograms[0x92] = nullptr;
   uPrograms[0x93] = nullptr;
   uPrograms[0x94] = &CPU::STYZeroPageX;
   uPrograms[0x95] = &CPU::STAZeroPageX;
   uPrograms[0x96] = &CPU::STXZeroPageY;
   uPrograms[0x97] = nullptr;
   uPrograms[0x98] = &CPU::TYAImplicit;
   uPrograms[0x99] = &CPU::STAAbsoluteY;
   uPrograms[0x9a] = &CPU::TXSImplicit;
   uPrograms[0x9b] = nullptr;
   uPrograms[0x9c] = nullptr;
   uPrograms[0x9d] = &CPU::STAAbsoluteX;
   uPrograms[0x9e] = nullptr;
   uPrograms[0x9f] = nullptr;
   uPrograms[0xa0] = &CPU::LDYImmediate;
   uPrograms[0xa1] = &CPU::LDAIndexedIndirect;
   uPrograms[0xa2] = &CPU::LDXImmediate;
   uPrograms[0xa3] = nullptr;
   uPrograms[0xa4] = &CPU::LDYZeroPage;
   uPrograms[0xa5] = &CPU::LDAZeroPage;
   uPrograms[0xa6] = &CPU::LDXZeroPage;
   uPrograms[0xa7] = nullptr;
   uPrograms[0xa8] = &CPU::TAYImplicit;
   uPrograms[0xa9] = &CPU::LDAImmediate;
   uPrograms[0xaa] = &CPU::TAXImplicit;
   uPrograms[0xab] = nullptr;
   uPrograms[0xac] = &CPU::LDYAbsolute;
   uPrograms[0xad] = &CPU::LDAAbsolute;
   uPrograms[0xae] = &CPU::LDXAbsolute;
   uPrograms[0xaf] = nullptr;
   uPrograms[0xb0] = &CPU::BCSRelative;
   uPrograms[0xb1] = &CPU::LDAIndirectIndexed;
   uPrograms[0xb2] = nullptr;
   uPrograms[0xb3] = nullptr;
   uPrograms[0xb4] = &CPU::LDYZeroPageX;
   uPrograms[0xb5] = &CPU::LDAZeroPageX;
   uPrograms[0xb6] = &CPU::LDXZeroPageY;
   uPrograms[0xb7] = nullptr;
   uPrograms[0xb8] = &CPU::CLVImplicit;
   uPrograms[0xb9] = &CPU::LDAAbsoluteY;
   uPrograms[0xba] = &CPU::TSXImplicit;
   uPrograms[0xbb] = nullptr;
   uPrograms[0xbc] = &CPU::LDYAbsoluteX;
   uPrograms[0xbd] = &CPU::LDAAbsoluteX;
   uPrograms[0xbe] = &CPU::LDXAbsoluteY;
   uPrograms[0xbf] = nullptr;
   uPrograms[0xc0] = &CPU::CPYImmediate;
   uPrograms[0xc1] = &CPU::CMPIndexedIndirect;
   uPrograms[0xc2] = nullptr;
   uPrograms[0xc3] = nullptr;
   uPrograms[0xc4] = &CPU::CPYZeroPage;
   uPrograms[0xc5] = &CPU::CMPZeroPage;
   uPrograms[0xc6] = &CPU::DECZeroPage;
   uPrograms[0xc7] = nullptr;
   uPrograms[0xc8] = &CPU::INYImplicit;
   uPrograms[0xc9] = &CPU::CMPImmediate;
   uPrograms[0xca] = &CPU::DEXImplicit;
   uPrograms[0xcb] = nullptr;
   uPrograms[0xcc] = &CPU::CPYAbsolute;
   uPrograms[0xcd] = &CPU::CMPAbsolute;
   uPrograms[0xce] = &CPU::DECAbsolute;
   uPrograms[0xcf] = nullptr;
   uPrograms[0xd0] = &CPU::BNERelative;
   uPrograms[0xd1] = &CPU::CMPIndirectIndexed;
   uPrograms[0xd2] = nullptr;
   uPrograms[0xd3] = nullptr;
   uPrograms[0xd4] = nullptr;
   uPrograms[0xd5] = &CPU::CMPZeroPageX;
   uPrograms[0xd6] = &CPU::DECZeroPageX;
   uPrograms[0xd7] = nullptr;
   uPrograms[0xd8] = &CPU::CLDImplicit;
   uPrograms[0xd9] = &CPU::CMPAbsoluteY;
   uPrograms[0xda] = nullptr;
   uPrograms[0xdb] = nullptr;
   uPrograms[0xdc] = nullptr;
   uPrograms[0xdd] = &CPU::CMPAbsoluteX;
   uPrograms[0xde] = &CPU::DECAbsoluteX;
   uPrograms[0xdf] = nullptr;
   uPrograms[0xe0] = &CPU::CPXImmediate;
   uPrograms[0xe1] = &CPU::SBCIndexedIndirect;
   uPrograms[0xe2] = nullptr;
   uPrograms[0xe3] = nullptr;
   uPrograms[0xe4] = &CPU::CPXZeroPage;
   uPrograms[0xe5] = &CPU::SBCZeroPage;
   uPrograms[0xe6] = &CPU::INCZeroPage;
   uPrograms[0xe7] = nullptr;
   uPrograms[0xe8] = &CPU::INXImplicit;
   uPrograms[0xe9] = &CPU::SBCImmediate;
   uPrograms[0xea] = &CPU::NOPImplicit;
   uPrograms[0xeb] = nullptr;
   uPrograms[0xec] = &CPU::CPXAbsolute;
   uPrograms[0xed] = &CPU::SBCAbsolute;
   uPrograms[0xee] = &CPU::INCAbsolute;
   uPrograms[0xef] = nullptr;
   uPrograms[0xf0] = &CPU::BEQRelative;
   uPrograms[0xf1] = &CPU::SBCIndirectIndexed;
   uPrograms[0xf2] = nullptr;
   uPrograms[0xf3] = nullptr;
   uPrograms[0xf4] = nullptr;
   uPrograms[0xf5] = &CPU::SBCZeroPageX;
   uPrograms[0xf6] = &CPU::INCZeroPageX;
   uPrograms[0xf7] = nullptr;
   uPrograms[0xf8] = &CPU::SEDImplicit;
   uPrograms[0xf9] = &CPU::SBCAbsoluteY;
   uPrograms[0xfa] = nullptr;
   uPrograms[0xfb] = nullptr;
   uPrograms[0xfc] = nullptr;
   uPrograms[0xfd] = &CPU::SBCAbsoluteX;
   uPrograms[0xfe] = &CPU::INCAbsoluteX;
   uPrograms[0xff] = nullptr;
}

void CPU::tick()
{
  ppu->tick();
  ppu->tick();
  ppu->tick();
  ++cycleCount;
}

void CPU::execute()
{
   if(resetSignal) {
      ppu->reset();
      resetSignal = false;
      tick();
      tick();
      tick();
      tick();
      PC = ram->read(0xFFFC);
      tick();
      PC |= ram->read(0xFFFD) << 8;
      tick();
      return;
   }

   if(ppu->NMISignalOn()) { //NMI enabled and vBlank
      tick();
      tick();
      ram->write(0x100 + S--, PC >> 8);
      tick();
      ram->write(0x100 + S--, PC);
      tick();
      ram->write(0x100 + S--, P.raw | 0x20);
      tick();
      PC = ram->read(0xFFFA);
      tick();
      PC |= ram->read(0xFFFB) << 8;
      tick();
      return;
   }

   uint8_t op = ram->read(PC++);
   tick();

   (this->*uPrograms[op])();   
}

void CPU::loadRAM(CPURam* ram)
{
   this->ram = ram;
   PC = 0xC000;
}


void CPU::ADCImmediate()
{
   addr = PC++;
   memoryByte = ram->read(addr);
   tick();

   uint16_t result = A + memoryByte + (P.C.getValue() ? 1 : 0);

   P.C = ((result > 0xFF) ? true : false);

   bool a = A & 0x80;
   bool b = (memoryByte + (P.C.getValue() ? 1 : 0)) & 0x80;
   bool r = result & 0x80;

   P.V = (((r & !(a | b)) | (!r & a & b)) ? true : false);

   temp = static_cast<uint8_t>(result);
   A = temp;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::ADCZeroPage()
{
   addr = ram->read(PC++);
   tick();
   memoryByte = ram->read(addr);
   tick();

   uint16_t result = A + memoryByte + (P.C.getValue() ? 1 : 0);

   P.C = ((result > 0xFF) ? true : false);

   bool a = A & 0x80;
   bool b = (memoryByte + (P.C.getValue() ? 1 : 0)) & 0x80;
   bool r = result & 0x80;

   P.V = (((r & !(a | b)) | (!r & a & b)) ? true : false);

   temp = static_cast<uint8_t>(result);
   A = temp;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::ADCZeroPageX()
{
   addr = static_cast<uint8_t>(ram->read(PC++) + X);
   tick();
   tick();
   memoryByte = ram->read(addr);
   tick();

   uint16_t result = A + memoryByte + (P.C.getValue() ? 1 : 0);

   P.C = ((result > 0xFF) ? true : false);

   bool a = A & 0x80;
   bool b = (memoryByte + (P.C.getValue() ? 1 : 0)) & 0x80;
   bool r = result & 0x80;

   P.V = (((r & !(a | b)) | (!r & a & b)) ? true : false);

   temp = static_cast<uint8_t>(result);
   A = temp;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::ADCAbsolute()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   memoryByte = ram->read(addr);
   tick();

   uint16_t result = A + memoryByte + (P.C.getValue() ? 1 : 0);

   P.C = ((result > 0xFF) ? true : false);

   bool a = A & 0x80;
   bool b = (memoryByte + (P.C.getValue() ? 1 : 0)) & 0x80;
   bool r = result & 0x80;

   P.V = (((r & !(a | b)) | (!r & a & b)) ? true : false);

   temp = static_cast<uint8_t>(result);
   A = temp;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::ADCAbsoluteX()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   if((addr & 0x00FF) + X > 0xFF) {
      tick();
   }
   addr += X;
   memoryByte = ram->read(addr);
   tick();

   uint16_t result = A + memoryByte + (P.C.getValue() ? 1 : 0);

   P.C = ((result > 0xFF) ? true : false);

   bool a = A & 0x80;
   bool b = (memoryByte + (P.C.getValue() ? 1 : 0)) & 0x80;
   bool r = result & 0x80;

   P.V = (((r & !(a | b)) | (!r & a & b)) ? true : false);

   temp = static_cast<uint8_t>(result);
   A = temp;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::ADCAbsoluteY()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   if((addr & 0x00FF) + Y > 0xFF) {
      tick();
   }
   addr += Y;
   memoryByte = ram->read(addr);
   tick();

   uint16_t result = A + memoryByte + (P.C.getValue() ? 1 : 0);

   P.C = ((result > 0xFF) ? true : false);

   bool a = A & 0x80;
   bool b = (memoryByte + (P.C.getValue() ? 1 : 0)) & 0x80;
   bool r = result & 0x80;

   P.V = (((r & !(a | b)) | (!r & a & b)) ? true : false);

   temp = static_cast<uint8_t>(result);
   A = temp;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::ADCIndexedIndirect()
{
   addr = static_cast<uint8_t>(ram->read(PC++) + X);
   tick();
   tick();
   addr = ram->read(addr) | (ram->read(static_cast<uint8_t>(addr + 1)) << 8);
   tick();
   tick();
   memoryByte = ram->read(addr);
   tick();

   uint16_t result = A + memoryByte + (P.C.getValue() ? 1 : 0);

   P.C = ((result > 0xFF) ? true : false);

   bool a = A & 0x80;
   bool b = (memoryByte + (P.C.getValue() ? 1 : 0)) & 0x80;
   bool r = result & 0x80;

   P.V = (((r & !(a | b)) | (!r & a & b)) ? true : false);

   temp = static_cast<uint8_t>(result);
   A = temp;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::ADCIndirectIndexed()
{
   addr = ram->read(PC++);
   tick();
   addr = ram->read(addr) | (ram->read((uint8_t)(addr+1)) << 8);
   tick();
   tick();
   if((addr & 0x00FF) + Y > 0xFF) {
      tick();
   }
   addr += Y;
   memoryByte = ram->read(addr);
   tick();

   uint16_t result = A + memoryByte + (P.C.getValue() ? 1 : 0);

   P.C = ((result > 0xFF) ? true : false);

   bool a = A & 0x80;
   bool b = (memoryByte + (P.C.getValue() ? 1 : 0)) & 0x80;
   bool r = result & 0x80;

   P.V = (((r & !(a | b)) | (!r & a & b)) ? true : false);

   temp = static_cast<uint8_t>(result);
   A = temp;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::ANDImmediate()
{
   addr = PC++;
   memoryByte = ram->read(addr);
   tick();

   A &= memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::ANDZeroPage()
{
   addr = ram->read(PC++);
   tick();
   memoryByte = ram->read(addr);
   tick();

   A &= memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::ANDZeroPageX()
{
   addr = static_cast<uint8_t>(ram->read(PC++) + X);
   tick();
   tick();
   memoryByte = ram->read(addr);
   tick();

   A &= memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::ANDAbsolute()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   memoryByte = ram->read(addr);
   tick();

   A &= memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::ANDAbsoluteX()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   if((addr & 0x00FF) + X > 0xFF) {
      tick();
   }
   addr += X;
   memoryByte = ram->read(addr);
   tick();

   A &= memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::ANDAbsoluteY()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   if((addr & 0x00FF) + Y > 0xFF) {
      tick();
   }
   addr += Y;
   memoryByte = ram->read(addr);
   tick();

   A &= memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::ANDIndexedIndirect()
{
   addr = static_cast<uint8_t>(ram->read(PC++) + X);
   tick();
   tick();
   addr = ram->read(addr) | (ram->read(static_cast<uint8_t>(addr + 1)) << 8);
   tick();
   tick();
   memoryByte = ram->read(addr);
   tick();

   A &= memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::ANDIndirectIndexed()
{
   addr = ram->read(PC++);
   tick();
   addr = ram->read(addr) | (ram->read((uint8_t)(addr+1)) << 8);
   tick();
   tick();
   if((addr & 0x00FF) + Y > 0xFF) {
      tick();
   }
   addr += Y;
   memoryByte = ram->read(addr);
   tick();

   A &= memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::ASLAccumulator()
{
   memoryByte = A;

   P.C = static_cast<bool>(memoryByte & 0x80);
   memoryByte <<= 1;
   temp = memoryByte;

   A = memoryByte;

   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::ASLZeroPage()
{
   addr = ram->read(PC++);
   tick();
   memoryByte = ram->read(addr);
   tick();

   P.C = static_cast<bool>(memoryByte & 0x80);
   memoryByte <<= 1;
   temp = memoryByte;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   ram->write(addr, memoryByte);
   tick();
}

void CPU::ASLZeroPageX()
{
   addr = static_cast<uint8_t>(ram->read(PC++) + X);
   tick();
   tick();
   memoryByte = ram->read(addr);
   tick();

   P.C = static_cast<bool>(memoryByte & 0x80);
   memoryByte <<= 1;
   temp = memoryByte;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   ram->write(addr, memoryByte);
   tick();
}

void CPU::ASLAbsolute()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   memoryByte = ram->read(addr);
   tick();

   P.C = static_cast<bool>(memoryByte & 0x80);
   memoryByte <<= 1;
   temp = memoryByte;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   ram->write(addr, memoryByte);
   tick();
}

void CPU::ASLAbsoluteX()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   if((addr & 0x00FF) + X > 0xFF) {
      tick();
   }
   addr += X;
   memoryByte = ram->read(addr);
   tick();

   P.C = static_cast<bool>(memoryByte & 0x80);
   memoryByte <<= 1;
   temp = memoryByte;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   ram->write(addr, memoryByte);
   tick();
}

void CPU::BCCRelative()
{

   condition = !P.C.getValue();

   if(condition) {
      temp = ram->read(PC++);
      tick();
      uint8_t PCL = static_cast<uint8_t>(PC);
      uint8_t res = PCL + static_cast<int8_t>(temp);
      tick();
      if(!(((PCL & 0x80) ^ (temp & 0x80)) || ((PCL & 0x80) == (res & 0x80)))) {
        tick(); 
      } 
      PC += static_cast<int8_t>(temp);
   } else {
      tick();
      ++PC;
   }
}

void CPU::BCSRelative()
{

   condition = P.C.getValue();

   if(condition) {
      temp = ram->read(PC++);
      tick();
      uint8_t PCL = static_cast<uint8_t>(PC);
      uint8_t res = PCL + static_cast<int8_t>(temp);
      tick();
      if(!(((PCL & 0x80) ^ (temp & 0x80)) || ((PCL & 0x80) == (res & 0x80)))) {
        tick(); 
      } 
      PC += static_cast<int8_t>(temp);
   } else {
      tick();
      ++PC;
   }
}

void CPU::BEQRelative()
{

   condition = P.Z.getValue();

   if(condition) {
      temp = ram->read(PC++);
      tick();
      uint8_t PCL = static_cast<uint8_t>(PC);
      uint8_t res = PCL + static_cast<int8_t>(temp);
      tick();
      if(!(((PCL & 0x80) ^ (temp & 0x80)) || ((PCL & 0x80) == (res & 0x80)))) {
        tick(); 
      } 
      PC += static_cast<int8_t>(temp);
   } else {
      tick();
      ++PC;
   }
}

void CPU::BITZeroPage()
{
   addr = static_cast<uint8_t>(ram->read(PC++));
   tick();
   memoryByte = ram->read(addr);
   tick();

   temp = A & memoryByte;
   P.Z = (temp == 0) ? 1 : 0;
   P.V = (memoryByte & 0x40) ? 1 : 0;
   P.N = (memoryByte & 0x80) ? 1 : 0;

}

void CPU::BITAbsolute()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   memoryByte = ram->read(addr);
   tick();

   temp = A & memoryByte;
   P.Z = (temp == 0) ? 1 : 0;
   P.V = (memoryByte & 0x40) ? 1 : 0;
   P.N = (memoryByte & 0x80) ? 1 : 0;

}

void CPU::BMIRelative()
{

   condition = P.N.getValue();

   if(condition) {
      temp = ram->read(PC++);
      tick();
      uint8_t PCL = static_cast<uint8_t>(PC);
      uint8_t res = PCL + static_cast<int8_t>(temp);
      tick();
      if(!(((PCL & 0x80) ^ (temp & 0x80)) || ((PCL & 0x80) == (res & 0x80)))) {
        tick(); 
      } 
      PC += static_cast<int8_t>(temp);
   } else {
      tick();
      ++PC;
   }
}

void CPU::BNERelative()
{

   condition = !P.Z.getValue();

   if(condition) {
      temp = ram->read(PC++);
      tick();
      uint8_t PCL = static_cast<uint8_t>(PC);
      uint8_t res = PCL + static_cast<int8_t>(temp);
      tick();
      if(!(((PCL & 0x80) ^ (temp & 0x80)) || ((PCL & 0x80) == (res & 0x80)))) {
        tick(); 
      } 
      PC += static_cast<int8_t>(temp);
   } else {
      tick();
      ++PC;
   }
}

void CPU::BPLRelative()
{

   condition = !P.N.getValue();

   if(condition) {
      temp = ram->read(PC++);
      tick();
      uint8_t PCL = static_cast<uint8_t>(PC);
      uint8_t res = PCL + static_cast<int8_t>(temp);
      tick();
      if(!(((PCL & 0x80) ^ (temp & 0x80)) || ((PCL & 0x80) == (res & 0x80)))) {
        tick(); 
      } 
      PC += static_cast<int8_t>(temp);
   } else {
      tick();
      ++PC;
   }
}

void CPU::BRKImplicit()
{
   

   ++PC;
   tick();
   ram->write(0x100 + S--, PC >> 8);
   tick();
   ram->write(0x100 + S--, PC);
   tick();
   ram->write(0x100 + S--, P.raw | 0x30);
   P.B = true;
   tick();
   PC = ram->read(0xFFFE);
   tick();
   PC |= ram->read(0xFFFF) << 8;
   tick();

}

void CPU::BVCRelative()
{

   condition = !P.V.getValue();

   if(condition) {
      temp = ram->read(PC++);
      tick();
      uint8_t PCL = static_cast<uint8_t>(PC);
      uint8_t res = PCL + static_cast<int8_t>(temp);
      tick();
      if(!(((PCL & 0x80) ^ (temp & 0x80)) || ((PCL & 0x80) == (res & 0x80)))) {
        tick(); 
      } 
      PC += static_cast<int8_t>(temp);
   } else {
      tick();
      ++PC;
   }
}

void CPU::BVSRelative()
{

   condition = P.V.getValue();

   if(condition) {
      temp = ram->read(PC++);
      tick();
      uint8_t PCL = static_cast<uint8_t>(PC);
      uint8_t res = PCL + static_cast<int8_t>(temp);
      tick();
      if(!(((PCL & 0x80) ^ (temp & 0x80)) || ((PCL & 0x80) == (res & 0x80)))) {
        tick(); 
      } 
      PC += static_cast<int8_t>(temp);
   } else {
      tick();
      ++PC;
   }
}

void CPU::CLCImplicit()
{
   

   P.C = false;
   tick();

}

void CPU::CLDImplicit()
{
   

   P.D = false;
   tick();

}

void CPU::CLIImplicit()
{
   

   P.I = false;
   tick();

}

void CPU::CLVImplicit()
{
   

   P.V = false;
   tick();

}

void CPU::CMPImmediate()
{
   addr = PC++;
   memoryByte = ram->read(addr);
   tick();

   temp = A-memoryByte;
   P.C = (A >= memoryByte) ? 1 : 0;
   P.Z = !static_cast<bool>(temp);
   P.N = static_cast<bool>(temp & 0x80);

}

void CPU::CMPZeroPage()
{
   addr = ram->read(PC++);
   tick();
   memoryByte = ram->read(addr);
   tick();

   temp = A-memoryByte;
   P.C = (A >= memoryByte) ? 1 : 0;
   P.Z = !static_cast<bool>(temp);
   P.N = static_cast<bool>(temp & 0x80);

}

void CPU::CMPZeroPageX()
{
   addr = static_cast<uint8_t>(ram->read(PC++) + X);
   tick();
   tick();
   memoryByte = ram->read(addr);
   tick();

   temp = A-memoryByte;
   P.C = (A >= memoryByte) ? 1 : 0;
   P.Z = !static_cast<bool>(temp);
   P.N = static_cast<bool>(temp & 0x80);

}

void CPU::CMPAbsolute()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   memoryByte = ram->read(addr);
   tick();

   temp = A-memoryByte;
   P.C = (A >= memoryByte) ? 1 : 0;
   P.Z = !static_cast<bool>(temp);
   P.N = static_cast<bool>(temp & 0x80);

}

void CPU::CMPAbsoluteX()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   if((addr & 0x00FF) + X > 0xFF) {
      tick();
   }
   addr += X;
   memoryByte = ram->read(addr);
   tick();

   temp = A-memoryByte;
   P.C = (A >= memoryByte) ? 1 : 0;
   P.Z = !static_cast<bool>(temp);
   P.N = static_cast<bool>(temp & 0x80);

}

void CPU::CMPAbsoluteY()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   if((addr & 0x00FF) + Y > 0xFF) {
      tick();
   }
   addr += Y;
   memoryByte = ram->read(addr);
   tick();

   temp = A-memoryByte;
   P.C = (A >= memoryByte) ? 1 : 0;
   P.Z = !static_cast<bool>(temp);
   P.N = static_cast<bool>(temp & 0x80);

}

void CPU::CMPIndexedIndirect()
{
   addr = static_cast<uint8_t>(ram->read(PC++) + X);
   tick();
   tick();
   addr = ram->read(addr) | (ram->read(static_cast<uint8_t>(addr + 1)) << 8);
   tick();
   tick();
   memoryByte = ram->read(addr);
   tick();

   temp = A-memoryByte;
   P.C = (A >= memoryByte) ? 1 : 0;
   P.Z = !static_cast<bool>(temp);
   P.N = static_cast<bool>(temp & 0x80);

}

void CPU::CMPIndirectIndexed()
{
   addr = ram->read(PC++);
   tick();
   addr = ram->read(addr) | (ram->read((uint8_t)(addr+1)) << 8);
   tick();
   tick();
   if((addr & 0x00FF) + Y > 0xFF) {
      tick();
   }
   addr += Y;
   memoryByte = ram->read(addr);
   tick();

   temp = A-memoryByte;
   P.C = (A >= memoryByte) ? 1 : 0;
   P.Z = !static_cast<bool>(temp);
   P.N = static_cast<bool>(temp & 0x80);

}

void CPU::CPXImmediate()
{
   addr = PC++;
   memoryByte = ram->read(addr);
   tick();

   temp = X-memoryByte;
   P.C = (X >= memoryByte) ? 1 : 0;
   P.Z = !static_cast<bool>(temp);
   P.N = static_cast<bool>(temp & 0x80);

}

void CPU::CPXZeroPage()
{
   addr = ram->read(PC++);
   tick();
   memoryByte = ram->read(addr);
   tick();

   temp = X-memoryByte;
   P.C = (X >= memoryByte) ? 1 : 0;
   P.Z = !static_cast<bool>(temp);
   P.N = static_cast<bool>(temp & 0x80);

}

void CPU::CPXAbsolute()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   memoryByte = ram->read(addr);
   tick();

   temp = X-memoryByte;
   P.C = (X >= memoryByte) ? 1 : 0;
   P.Z = !static_cast<bool>(temp);
   P.N = static_cast<bool>(temp & 0x80);

}

void CPU::CPYImmediate()
{
   addr = PC++;
   memoryByte = ram->read(addr);
   tick();

   temp = Y-memoryByte;
   P.C = (Y >= memoryByte) ? 1 : 0;
   P.Z = !static_cast<bool>(temp);
   P.N = static_cast<bool>(temp & 0x80);

}

void CPU::CPYZeroPage()
{
   addr = ram->read(PC++);
   tick();
   memoryByte = ram->read(addr);
   tick();

   temp = Y-memoryByte;
   P.C = (Y >= memoryByte) ? 1 : 0;
   P.Z = !static_cast<bool>(temp);
   P.N = static_cast<bool>(temp & 0x80);

}

void CPU::CPYAbsolute()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   memoryByte = ram->read(addr);
   tick();

   temp = Y-memoryByte;
   P.C = (Y >= memoryByte) ? 1 : 0;
   P.Z = !static_cast<bool>(temp);
   P.N = static_cast<bool>(temp & 0x80);

}

void CPU::DECZeroPage()
{
   addr = ram->read(PC++);
   tick();
   memoryByte = ram->read(addr);
   tick();

   --memoryByte;
   temp = memoryByte;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   ram->write(addr, memoryByte);
   tick();
}

void CPU::DECZeroPageX()
{
   addr = static_cast<uint8_t>(ram->read(PC++) + X);
   tick();
   tick();
   memoryByte = ram->read(addr);
   tick();

   --memoryByte;
   temp = memoryByte;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   ram->write(addr, memoryByte);
   tick();
}

void CPU::DECAbsolute()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   memoryByte = ram->read(addr);
   tick();

   --memoryByte;
   temp = memoryByte;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   ram->write(addr, memoryByte);
   tick();
}

void CPU::DECAbsoluteX()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   if((addr & 0x00FF) + X > 0xFF) {
      tick();
   }
   addr += X;
   memoryByte = ram->read(addr);
   tick();

   --memoryByte;
   temp = memoryByte;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   ram->write(addr, memoryByte);
   tick();
}

void CPU::DEXImplicit()
{
   --X;
   tick();
   temp = X;

   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::DEYImplicit()
{
   --Y;
   tick();
   temp = Y;

   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::EORImmediate()
{
   addr = PC++;
   memoryByte = ram->read(addr);
   tick();

   A ^= memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::EORZeroPage()
{
   addr = ram->read(PC++);
   tick();
   memoryByte = ram->read(addr);
   tick();

   A ^= memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::EORZeroPageX()
{
   addr = static_cast<uint8_t>(ram->read(PC++) + X);
   tick();
   tick();
   memoryByte = ram->read(addr);
   tick();

   A ^= memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::EORAbsolute()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   memoryByte = ram->read(addr);
   tick();

   A ^= memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::EORAbsoluteX()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   if((addr & 0x00FF) + X > 0xFF) {
      tick();
   }
   addr += X;
   memoryByte = ram->read(addr);
   tick();

   A ^= memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::EORAbsoluteY()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   if((addr & 0x00FF) + Y > 0xFF) {
      tick();
   }
   addr += Y;
   memoryByte = ram->read(addr);
   tick();

   A ^= memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::EORIndexedIndirect()
{
   addr = static_cast<uint8_t>(ram->read(PC++) + X);
   tick();
   tick();
   addr = ram->read(addr) | (ram->read(static_cast<uint8_t>(addr + 1)) << 8);
   tick();
   tick();
   memoryByte = ram->read(addr);
   tick();

   A ^= memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::EORIndirectIndexed()
{
   addr = ram->read(PC++);
   tick();
   addr = ram->read(addr) | (ram->read((uint8_t)(addr+1)) << 8);
   tick();
   tick();
   if((addr & 0x00FF) + Y > 0xFF) {
      tick();
   }
   addr += Y;
   memoryByte = ram->read(addr);
   tick();

   A ^= memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::INCZeroPage()
{
   addr = ram->read(PC++);
   tick();
   memoryByte = ram->read(addr);
   tick();

   ++memoryByte;
   temp = memoryByte;
   tick();

   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   ram->write(addr, memoryByte);
   tick();
}

void CPU::INCZeroPageX()
{
   addr = static_cast<uint8_t>(ram->read(PC++) + X);
   tick();
   tick();
   memoryByte = ram->read(addr);
   tick();

   ++memoryByte;
   temp = memoryByte;
   tick();


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   ram->write(addr, memoryByte);
   tick();
}

void CPU::INCAbsolute()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   memoryByte = ram->read(addr);
   tick();

   ++memoryByte;
   temp = memoryByte;
   tick();


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   ram->write(addr, memoryByte);
   tick();
}

void CPU::INCAbsoluteX()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   if((addr & 0x00FF) + X > 0xFF) {
      tick();
   }
   addr += X;
   memoryByte = ram->read(addr);
   tick();

   ++memoryByte;
   temp = memoryByte;
   tick();


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   ram->write(addr, memoryByte);
   tick();
}

void CPU::INXImplicit()
{
   

   ++X;
   tick();
   temp = X;
   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false; 
}

void CPU::INYImplicit()
{
   ++Y;
   tick();
   temp = Y;
   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false; 
}

void CPU::JMPAbsolute()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();

   PC = addr;

}

void CPU::JMPIndirect()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   uint8_t low = ram->read(addr);
   tick();
   uint8_t high = 0;
   if((addr & 0x00FF) == 0xFF) {
      high = ram->read(addr & 0xFF00);
   } else {
      high = ram->read(addr + 1);
   }
   tick();  
   addr = low | (high << 8);

   PC = addr;

}

void CPU::JSRAbsolute()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC) << 8;
   tick();

   ram->write(0x100 + S--, PC >> 8);
   tick();
   ram->write(0x100 + S--, PC);
   tick();
   PC = addr;
   tick();

}

void CPU::LDAImmediate()
{
   addr = PC++;
   memoryByte = ram->read(addr);
   tick();
   A = memoryByte;

   temp = A;

   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::LDAZeroPage()
{
   addr = ram->read(PC++);
   tick();
   memoryByte = ram->read(addr);
   tick();

   A = memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::LDAZeroPageX()
{
   addr = static_cast<uint8_t>(ram->read(PC++) + X);
   tick();
   tick();
   memoryByte = ram->read(addr);
   tick();

   A = memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::LDAAbsolute()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();

   memoryByte = ram->read(addr);
   tick();

   A = memoryByte;
   temp = A;

   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::LDAAbsoluteX()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;

   tick();
   if((addr & 0x00FF) + X > 0xFF) {
      tick();
	  memoryByte = ram->read(addr + X - 0x100);
   }
   addr += X;
   memoryByte = ram->read(addr);
   tick();

   A = memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::LDAAbsoluteY()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   if((addr & 0x00FF) + Y > 0xFF) {
      tick();
   }
   addr += Y;
   memoryByte = ram->read(addr);
   tick();

   A = memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::LDAIndexedIndirect()
{
   addr = static_cast<uint8_t>(ram->read(PC++) + X);
   tick();
   tick();
   addr = ram->read(addr) | (ram->read(static_cast<uint8_t>(addr + 1)) << 8);
   tick();
   tick();
   memoryByte = ram->read(addr);
   tick();

   A = memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::LDAIndirectIndexed()
{
   addr = ram->read(PC++);
   tick();
   addr = ram->read(addr) | (ram->read((uint8_t)(addr+1)) << 8);
   tick();
   tick();
   if((addr & 0x00FF) + Y > 0xFF) {
      tick();
	  ram->read(addr + Y - 0x100);
   }
   addr += Y;
   memoryByte = ram->read(addr);
   tick();

   A = memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::LDXImmediate()
{
   addr = PC++;
   memoryByte = ram->read(addr);

   tick();

   X = memoryByte;
   temp = X;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::LDXZeroPage()
{
   addr = ram->read(PC++);
   tick();
   memoryByte = ram->read(addr);
   tick();

   X = memoryByte;
   temp = X;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::LDXZeroPageY()
{
   addr = static_cast<uint8_t>(ram->read(PC++) + Y);
   tick();
   tick();
   memoryByte = ram->read(addr);
   tick();

   X = memoryByte;
   temp = X;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::LDXAbsolute()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   memoryByte = ram->read(addr);
   tick();

   X = memoryByte;
   temp = X;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::LDXAbsoluteY()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   if((addr & 0x00FF) + Y > 0xFF) {
      tick();
   }
   addr += Y;
   memoryByte = ram->read(addr);
   tick();

   X = memoryByte;
   temp = X;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::LDYImmediate()
{
   addr = PC++;
   memoryByte = ram->read(addr);
   tick();

   Y = memoryByte;
   temp = Y;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::LDYZeroPage()
{
   addr = ram->read(PC++);
   tick();
   memoryByte = ram->read(addr);
   tick();

   Y = memoryByte;
   temp = Y;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::LDYZeroPageX()
{
   addr = static_cast<uint8_t>(ram->read(PC++) + X);
   tick();
   tick();
   memoryByte = ram->read(addr);
   tick();

   Y = memoryByte;
   temp = Y;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::LDYAbsolute()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   memoryByte = ram->read(addr);
   tick();

   Y = memoryByte;
   temp = Y;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::LDYAbsoluteX()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   if((addr & 0x00FF) + X > 0xFF) {
      tick();
   }
   addr += X;
   memoryByte = ram->read(addr);
   tick();

   Y = memoryByte;
   temp = Y;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::LSRAccumulator()
{
   memoryByte = A;

   P.C = static_cast<bool>(memoryByte & 0x01);
   memoryByte >>= 1;
   temp = memoryByte;
   tick();
   A = memoryByte;

   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::LSRZeroPage()
{
   addr = ram->read(PC++);
   tick();
   memoryByte = ram->read(addr);
   tick();

   P.C = static_cast<bool>(memoryByte & 0x01);
   memoryByte >>= 1;
   temp = memoryByte;
   tick();

   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   ram->write(addr, memoryByte);
   tick();
}

void CPU::LSRZeroPageX()
{
   addr = static_cast<uint8_t>(ram->read(PC++) + X);
   tick();
   tick();
   memoryByte = ram->read(addr);
   tick();

   P.C = static_cast<bool>(memoryByte & 0x01);
   memoryByte >>= 1;
   temp = memoryByte;
   tick();

   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   ram->write(addr, memoryByte);
   tick();
}

void CPU::LSRAbsolute()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   memoryByte = ram->read(addr);
   tick();

   P.C = static_cast<bool>(memoryByte & 0x01);
   memoryByte >>= 1;
   temp = memoryByte;
   tick();

   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   ram->write(addr, memoryByte);
   tick();
}

void CPU::LSRAbsoluteX()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   if((addr & 0x00FF) + X > 0xFF) {
      tick();
   }
   addr += X;
   memoryByte = ram->read(addr);
   tick();

   P.C = static_cast<bool>(memoryByte & 0x01);
   memoryByte >>= 1;
   temp = memoryByte;
   tick();

   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   ram->write(addr, memoryByte);
   tick();
}

void CPU::NOPImplicit()
{
   tick();
}

void CPU::ORAImmediate()
{
   addr = PC++;
   memoryByte = ram->read(addr);
   tick();

   A |= memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::ORAZeroPage()
{
   addr = ram->read(PC++);
   tick();
   memoryByte = ram->read(addr);
   tick();

   A |= memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::ORAZeroPageX()
{
   addr = static_cast<uint8_t>(ram->read(PC++) + X);
   tick();
   tick();
   memoryByte = ram->read(addr);
   tick();

   A |= memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::ORAAbsolute()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   memoryByte = ram->read(addr);
   tick();

   A |= memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::ORAAbsoluteX()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   if((addr & 0x00FF) + X > 0xFF) {
      tick();
   }
   addr += X;
   memoryByte = ram->read(addr);
   tick();

   A |= memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::ORAAbsoluteY()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   if((addr & 0x00FF) + Y > 0xFF) {
      tick();
   }
   addr += Y;
   memoryByte = ram->read(addr);
   tick();

   A |= memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::ORAIndexedIndirect()
{
   addr = static_cast<uint8_t>(ram->read(PC++) + X);
   tick();
   tick();
   addr = ram->read(addr) | (ram->read(static_cast<uint8_t>(addr + 1)) << 8);
   tick();
   tick();
   memoryByte = ram->read(addr);
   tick();

   A |= memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::ORAIndirectIndexed()
{
   addr = ram->read(PC++);
   tick();
   addr = ram->read(addr) | (ram->read((uint8_t)(addr+1)) << 8);
   tick();
   tick();
   if((addr & 0x00FF) + Y > 0xFF) {
      tick();
   }
   addr += Y;
   memoryByte = ram->read(addr);
   tick();

   A |= memoryByte;
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::PHAImplicit()
{
   

   tick();
   ram->write(0x100 + S--, A);
   tick();

}

void CPU::PHPImplicit()
{
   

   tick();
   ram->write(0x100 + S--, P.raw | 0x30);
   tick();

}

void CPU::PLAImplicit()
{
   

   tick();
   ++S;
   tick();
   A = ram->read(0x100 + S);
   tick();
   temp = A;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::PLPImplicit()
{
   

   tick();
   ++S;
   tick();
   P.raw = ram->read(0x100 + S);
   tick();

}

void CPU::ROLImplicit()
{
   memoryByte = A;

   uint8_t carry = (P.C.getValue() ? 1 : 0);
   P.C = static_cast<bool>(memoryByte & 0x80);
   memoryByte <<= 1;
   memoryByte += carry;
   temp = memoryByte;
   tick();

   A = memoryByte;

   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::ROLZeroPage()
{
   addr = ram->read(PC++);
   tick();
   memoryByte = ram->read(addr);
   tick();

   uint8_t carry = (P.C.getValue() ? 1 : 0);
   P.C = static_cast<bool>(memoryByte & 0x80);
   memoryByte <<= 1;
   memoryByte += carry;
   temp = memoryByte;
   tick();


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   ram->write(addr, memoryByte);
   tick();
}

void CPU::ROLZeroPageX()
{
   addr = static_cast<uint8_t>(ram->read(PC++) + X);
   tick();
   tick();
   memoryByte = ram->read(addr);
   tick();

   uint8_t carry = (P.C.getValue() ? 1 : 0);
   P.C = static_cast<bool>(memoryByte & 0x80);
   memoryByte <<= 1;
   memoryByte += carry;
   temp = memoryByte;
   tick();


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   ram->write(addr, memoryByte);
   tick();
}

void CPU::ROLAbsolute()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   memoryByte = ram->read(addr);
   tick();

   uint8_t carry = (P.C.getValue() ? 1 : 0);
   P.C = static_cast<bool>(memoryByte & 0x80);
   memoryByte <<= 1;
   memoryByte += carry;
   temp = memoryByte;
   tick();


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   ram->write(addr, memoryByte);
   tick();
}

void CPU::ROLAbsoluteX()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   if((addr & 0x00FF) + X > 0xFF) {
      tick();
	  ram->read(addr + X - 0x100);
   } else {
	   ram->read(addr + X);
   }
   addr += X;
   memoryByte = ram->read(addr);
   tick();

   uint8_t carry = (P.C.getValue() ? 1 : 0);
   P.C = static_cast<bool>(memoryByte & 0x80);
   memoryByte <<= 1;
   memoryByte += carry;
   temp = memoryByte;
   tick();


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   ram->write(addr, memoryByte);
   tick();
}

void CPU::RORImplicit()
{
   memoryByte = A;

   uint8_t carry = (P.C.getValue() ? 1 : 0) << 7;
   P.C = static_cast<bool>(memoryByte & 0x01);
   memoryByte >>= 1;
   memoryByte += carry;
   temp = memoryByte;
   tick();

   A = memoryByte;

   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::RORZeroPage()
{
   addr = ram->read(PC++);
   tick();
   memoryByte = ram->read(addr);
   tick();

   uint8_t carry = (P.C.getValue() ? 1 : 0) << 7;
   P.C = static_cast<bool>(memoryByte & 0x01);
   memoryByte >>= 1;
   memoryByte += carry;
   temp = memoryByte;
   tick();


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   ram->write(addr, memoryByte);
   tick();
}

void CPU::RORZeroPageX()
{
   addr = static_cast<uint8_t>(ram->read(PC++) + X);
   tick();
   tick();
   memoryByte = ram->read(addr);
   tick();

   uint8_t carry = (P.C.getValue() ? 1 : 0) << 7;
   P.C = static_cast<bool>(memoryByte & 0x01);
   memoryByte >>= 1;
   memoryByte += carry;
   temp = memoryByte;
   tick();

   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   ram->write(addr, memoryByte);
   tick();
}

void CPU::RORAbsolute()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   memoryByte = ram->read(addr);
   tick();

   uint8_t carry = (P.C.getValue() ? 1 : 0) << 7;
   P.C = static_cast<bool>(memoryByte & 0x01);
   memoryByte >>= 1;
   memoryByte += carry;
   temp = memoryByte;
   tick();

   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   ram->write(addr, memoryByte);
   tick();
}

void CPU::RORAbsoluteX()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   if((addr & 0x00FF) + X > 0xFF) {
      tick();
   }
   addr += X;
   memoryByte = ram->read(addr);
   tick();

   uint8_t carry = (P.C.getValue() ? 1 : 0) << 7;
   P.C = static_cast<bool>(memoryByte & 0x01);
   memoryByte >>= 1;
   memoryByte += carry;
   temp = memoryByte;
   tick();

   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   ram->write(addr, memoryByte);
   tick();
}

void CPU::RTIImplicit()
{
   

   tick();
   ++S;
   tick();
   P.raw = ram->read(0x100 + S);
   ++S;
   tick();
   PC = ram->read(0x100 + S);
   ++S;
   tick();
   PC |= ram->read(0x100 + S) << 8;
   tick();

}

void CPU::RTSImplicit()
{
   

   tick();
   ++S;
   tick();
   PC = ram->read(0x100 + S);
   ++S;
   tick();
   PC |= ram->read(0x100 + S) << 8;
   tick();
   ++PC;
   tick();

}

void CPU::SBCImmediate()
{
   addr = PC++;
   memoryByte = ram->read(addr);
   tick();

   uint16_t result = A - memoryByte - (P.C.getValue() ? 0 : 1);

   memoryByte = -memoryByte - (P.C.getValue() ? 0 : 1);

   bool a = A & 0x80;
   bool b = (memoryByte & 0x80) ? true : false;
   bool r = result & 0x80;

   P.V = (((r & !(a | b)) | (!r & a & b)) ? true : false);

   temp = static_cast<uint8_t>(result);
   A = temp;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   P.C = ((result > 0xFF) ? false : true);
}

void CPU::SBCZeroPage()
{
   addr = ram->read(PC++);
   tick();
   memoryByte = ram->read(addr);
   tick();

   uint16_t result = A - memoryByte - (P.C.getValue() ? 0 : 1);

   memoryByte = -memoryByte - (P.C.getValue() ? 0 : 1);

   bool a = A & 0x80;
   bool b = (memoryByte & 0x80) ? true : false;
   bool r = result & 0x80;

   P.V = (((r & !(a | b)) | (!r & a & b)) ? true : false);

   temp = static_cast<uint8_t>(result);
   A = temp;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   P.C = ((result > 0xFF) ? false : true);
}

void CPU::SBCZeroPageX()
{
   addr = static_cast<uint8_t>(ram->read(PC++) + X);
   tick();
   tick();
   memoryByte = ram->read(addr);
   tick();

   uint16_t result = A - memoryByte - (P.C.getValue() ? 0 : 1);

   memoryByte = -memoryByte - (P.C.getValue() ? 0 : 1);

   bool a = A & 0x80;
   bool b = (memoryByte & 0x80) ? true : false;
   bool r = result & 0x80;

   P.V = (((r & !(a | b)) | (!r & a & b)) ? true : false);

   temp = static_cast<uint8_t>(result);
   A = temp;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   P.C = ((result > 0xFF) ? false : true);
}

void CPU::SBCAbsolute()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   memoryByte = ram->read(addr);
   tick();

   uint16_t result = A - memoryByte - (P.C.getValue() ? 0 : 1);

   memoryByte = -memoryByte - (P.C.getValue() ? 0 : 1);

   bool a = A & 0x80;
   bool b = (memoryByte & 0x80) ? true : false;
   bool r = result & 0x80;

   P.V = (((r & !(a | b)) | (!r & a & b)) ? true : false);

   temp = static_cast<uint8_t>(result);
   A = temp;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   P.C = ((result > 0xFF) ? false : true);
}

void CPU::SBCAbsoluteX()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   if((addr & 0x00FF) + X > 0xFF) {
      tick();
   }
   addr += X;
   memoryByte = ram->read(addr);
   tick();

   uint16_t result = A - memoryByte - (P.C.getValue() ? 0 : 1);

   memoryByte = -memoryByte - (P.C.getValue() ? 0 : 1);

   bool a = A & 0x80;
   bool b = (memoryByte & 0x80) ? true : false;
   bool r = result & 0x80;

   P.V = (((r & !(a | b)) | (!r & a & b)) ? true : false);

   temp = static_cast<uint8_t>(result);
   A = temp;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   P.C = ((result > 0xFF) ? false : true);
}

void CPU::SBCAbsoluteY()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   if((addr & 0x00FF) + Y > 0xFF) {
      tick();
   }
   addr += Y;
   memoryByte = ram->read(addr);
   tick();

   uint16_t result = A - memoryByte - (P.C.getValue() ? 0 : 1);

   memoryByte = -memoryByte - (P.C.getValue() ? 0 : 1);

   bool a = A & 0x80;
   bool b = (memoryByte & 0x80) ? true : false;
   bool r = result & 0x80;

   P.V = (((r & !(a | b)) | (!r & a & b)) ? true : false);

   temp = static_cast<uint8_t>(result);
   A = temp;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   P.C = ((result > 0xFF) ? false : true);
}

void CPU::SBCIndexedIndirect()
{
   addr = static_cast<uint8_t>(ram->read(PC++) + X);
   tick();
   tick();
   addr = ram->read(addr) | (ram->read(static_cast<uint8_t>(addr + 1)) << 8);
   tick();
   tick();
   memoryByte = ram->read(addr);
   tick();

   uint16_t result = A - memoryByte - (P.C.getValue() ? 0 : 1);

   memoryByte = -memoryByte - (P.C.getValue() ? 0 : 1);

   bool a = A & 0x80;
   bool b = (memoryByte & 0x80) ? true : false;
   bool r = result & 0x80;

   P.V = (((r & !(a | b)) | (!r & a & b)) ? true : false);

   temp = static_cast<uint8_t>(result);
   A = temp;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   P.C = ((result > 0xFF) ? false : true);
}

void CPU::SBCIndirectIndexed()
{
   addr = ram->read(PC++);
   tick();
   addr = ram->read(addr) | (ram->read((uint8_t)(addr+1)) << 8);
   tick();
   tick();
   if((addr & 0x00FF) + Y > 0xFF) {
      tick();
   }
   addr += Y;
   memoryByte = ram->read(addr);
   tick();

   uint16_t result = A - memoryByte - (P.C.getValue() ? 0 : 1);

   memoryByte = -memoryByte - (P.C.getValue() ? 0 : 1);

   bool a = A & 0x80;
   bool b = (memoryByte & 0x80) ? true : false;
   bool r = result & 0x80;

   P.V = (((r & !(a | b)) | (!r & a & b)) ? true : false);

   temp = static_cast<uint8_t>(result);
   A = temp;


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
   P.C = ((result > 0xFF) ? false : true);
}

void CPU::SECImplicit()
{
   

   P.C = true;
   tick();

}

void CPU::SEDImplicit()
{
   

   P.D = true;
   tick();

}

void CPU::SEIImplicit()
{
   

   P.I = true;
   tick();

}

void CPU::STAZeroPage()
{
   addr = ram->read(PC++);
   tick();

   memoryByte = A;

   ram->write(addr, memoryByte);
   tick();
}

void CPU::STAZeroPageX()
{
   addr = static_cast<uint8_t>(ram->read(PC++) + X);
   tick();
   tick();

   memoryByte = A;

   ram->write(addr, memoryByte);
   tick();
}

void CPU::STAAbsolute()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();

   memoryByte = A;

   ram->write(addr, memoryByte);
   tick();
}

void CPU::STAAbsoluteX()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   if((addr & 0x00FF) + X > 0xFF) {
      tick();
	  ram->read(addr + X - 0x100);
   }
   addr += X;

   memoryByte = A;

   ram->write(addr, memoryByte);
   tick();
}

void CPU::STAAbsoluteY()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();
   if((addr & 0x00FF) + Y > 0xFF) {
      tick();
   }
   addr += Y;

   memoryByte = A;

   ram->write(addr, memoryByte);
   tick();
}

void CPU::STAIndexedIndirect()
{
   addr = static_cast<uint8_t>(ram->read(PC++) + X);
   tick();
   tick();
   addr = ram->read(addr) | (ram->read(static_cast<uint8_t>(addr + 1)) << 8);
   tick();
   tick();

   memoryByte = A;

   ram->write(addr, memoryByte);
   tick();
}

void CPU::STAIndirectIndexed()
{
   addr = ram->read(PC++);
   tick();
   addr = ram->read(addr) | (ram->read(addr+1) << 8);
   tick();
   tick();
   if((addr & 0x00FF) + Y > 0xFF) {
      tick();
	  ram->read(addr + Y - 0x100);
   }
   addr += Y;

   memoryByte = A;

   ram->write(addr, memoryByte);
   tick();
}

void CPU::STXZeroPage()
{
   addr = ram->read(PC++);
   tick();

   memoryByte = X;

   ram->write(addr, memoryByte);
   tick();
}

void CPU::STXZeroPageY()
{
   addr = static_cast<uint8_t>(ram->read(PC++) + Y);
   tick();
   tick();

   memoryByte = X;

   ram->write(addr, memoryByte);
   tick();
}

void CPU::STXAbsolute()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();

   memoryByte = X;

   ram->write(addr, memoryByte);
   tick();
}

void CPU::STYZeroPage()
{
   addr = ram->read(PC++);
   tick();

   memoryByte = Y;

   ram->write(addr, memoryByte);
   tick();
}

void CPU::STYZeroPageX()
{
   addr = static_cast<uint8_t>(ram->read(PC++) + X);
   tick();
   tick();

   memoryByte = Y;

   ram->write(addr, memoryByte);
   tick();
}

void CPU::STYAbsolute()
{
   addr = ram->read(PC++);
   tick();
   addr |= ram->read(PC++) << 8;
   tick();

   memoryByte = Y;

   ram->write(addr, memoryByte);
   tick();
}

void CPU::TAXImplicit()
{
   

   X = A;
   temp = X;
   tick();


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::TAYImplicit()
{
   

   Y = A;
   temp = Y;
   tick();


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::TSXImplicit()
{
   

   X = S;
   temp = X;
   tick();


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::TXAImplicit()
{
   

   A = X;
   temp = X;
   tick();


   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

void CPU::TXSImplicit()
{
   

   S = X;
   temp = S;
   tick();
}

void CPU::TYAImplicit()
{
   

   A = Y;
   temp = A;
   tick();

   P.Z = (temp == 0) ? true : false;
   P.N = (temp & 0x80) ? true : false;
}

