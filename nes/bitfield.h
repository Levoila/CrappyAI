#ifndef BITFIELD_H
#define BITFIELD_H

#include <stdint.h>

template <unsigned int N, typename T = uint8_t>
struct Bit
{
   T data;
   enum {mask = (1u << N)};

   Bit& operator=(bool val)
   {
      data = (val == true) ? (data | mask) : (data & ~mask);
	  return *this;
   }

   bool operator==(bool b)
   {
      return (data & mask) == b;
   }

   bool getValue()
   {
      return data & mask;
   }
};

union Bitfield
{
   uint8_t raw;
   Bit<0> C;
   Bit<1> Z;
   Bit<2> I;
   Bit<3> D;
   Bit<4> B;
   Bit<6> V;
   Bit<7> N;
};

#endif
