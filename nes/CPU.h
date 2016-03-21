#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <vector>

#include "bitfield.h"
#include "CPURam.h"
#include "PPU.h"

class CPURam;

class CPU
{
   public:
      CPU(PPU* p);
      void tick();
      void execute();
      void loadRAM(CPURam* ram);
      unsigned int cycleCount;
   private:

      //Registers
      uint16_t PC;
      uint8_t A, X, Y, S;
      Bitfield P;

      //Temporary variables for micro-programs
      uint16_t addr;
      uint8_t memoryByte, temp;
      bool condition;

      //Current micro-program
      uint8_t op;

      //true after reset or power up
      bool resetSignal;

      //List of micro-programs
      void (CPU::*uPrograms[0x100])();

      //RAM
      CPURam* ram;

      PPU* ppu;

      //Instruction set
      void ADCImmediate();
      void ADCZeroPage();
      void ADCZeroPageX();
      void ADCAbsolute();
      void ADCAbsoluteX();
      void ADCAbsoluteY();
      void ADCIndexedIndirect();
      void ADCIndirectIndexed();
      void ANDImmediate();
      void ANDZeroPage();
      void ANDZeroPageX();
      void ANDAbsolute();
      void ANDAbsoluteX();
      void ANDAbsoluteY();
      void ANDIndexedIndirect();
      void ANDIndirectIndexed();
      void ASLAccumulator();
      void ASLZeroPage();
      void ASLZeroPageX();
      void ASLAbsolute();
      void ASLAbsoluteX();
      void BCCRelative();
      void BCSRelative();
      void BEQRelative();
      void BITZeroPage();
      void BITAbsolute();
      void BMIRelative();
      void BNERelative();
      void BPLRelative();
      void BRKImplicit();
      void BVCRelative();
      void BVSRelative();
      void CLCImplicit();
      void CLDImplicit();
      void CLIImplicit();
      void CLVImplicit();
      void CMPImmediate();
      void CMPZeroPage();
      void CMPZeroPageX();
      void CMPAbsolute();
      void CMPAbsoluteX();
      void CMPAbsoluteY();
      void CMPIndexedIndirect();
      void CMPIndirectIndexed();
      void CPXImmediate();
      void CPXZeroPage();
      void CPXAbsolute();
      void CPYImmediate();
      void CPYZeroPage();
      void CPYAbsolute();
      void DECZeroPage();
      void DECZeroPageX();
      void DECAbsolute();
      void DECAbsoluteX();
      void DEXImplicit();
      void DEYImplicit();
      void EORImmediate();
      void EORZeroPage();
      void EORZeroPageX();
      void EORAbsolute();
      void EORAbsoluteX();
      void EORAbsoluteY();
      void EORIndexedIndirect();
      void EORIndirectIndexed();
      void INCZeroPage();
      void INCZeroPageX();
      void INCAbsolute();
      void INCAbsoluteX();
      void INXImplicit();
      void INYImplicit();
      void JMPAbsolute();
      void JMPIndirect();
      void JSRAbsolute();
      void LDAImmediate();
      void LDAZeroPage();
      void LDAZeroPageX();
      void LDAAbsolute();
      void LDAAbsoluteX();
      void LDAAbsoluteY();
      void LDAIndexedIndirect();
      void LDAIndirectIndexed();
      void LDXImmediate();
      void LDXZeroPage();
      void LDXZeroPageY();
      void LDXAbsolute();
      void LDXAbsoluteY();
      void LDYImmediate();
      void LDYZeroPage();
      void LDYZeroPageX();
      void LDYAbsolute();
      void LDYAbsoluteX();
      void LSRAccumulator();
      void LSRZeroPage();
      void LSRZeroPageX();
      void LSRAbsolute();
      void LSRAbsoluteX();
      void NOPImplicit();
      void ORAImmediate();
      void ORAZeroPage();
      void ORAZeroPageX();
      void ORAAbsolute();
      void ORAAbsoluteX();
      void ORAAbsoluteY();
      void ORAIndexedIndirect();
      void ORAIndirectIndexed();
      void PHAImplicit();
      void PHPImplicit();
      void PLAImplicit();
      void PLPImplicit();
      void ROLImplicit();
      void ROLZeroPage();
      void ROLZeroPageX();
      void ROLAbsolute();
      void ROLAbsoluteX();
      void RORImplicit();
      void RORZeroPage();
      void RORZeroPageX();
      void RORAbsolute();
      void RORAbsoluteX();
      void RTIImplicit();
      void RTSImplicit();
      void SBCImmediate();
      void SBCZeroPage();
      void SBCZeroPageX();
      void SBCAbsolute();
      void SBCAbsoluteX();
      void SBCAbsoluteY();
      void SBCIndexedIndirect();
      void SBCIndirectIndexed();
      void SECImplicit();
      void SEDImplicit();
      void SEIImplicit();
      void STAZeroPage();
      void STAZeroPageX();
      void STAAbsolute();
      void STAAbsoluteX();
      void STAAbsoluteY();
      void STAIndexedIndirect();
      void STAIndirectIndexed();
      void STXZeroPage();
      void STXZeroPageY();
      void STXAbsolute();
      void STYZeroPage();
      void STYZeroPageX();
      void STYAbsolute();
      void TAXImplicit();
      void TAYImplicit();
      void TSXImplicit();
      void TXAImplicit();
      void TXSImplicit();
      void TYAImplicit();
};

#endif
