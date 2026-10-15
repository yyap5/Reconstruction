#ifndef DATATYPES_HXX
#define DATATYPES_HXX

#include <RtypesCore.h>

struct four_momentum{
  Float_t px;
  Float_t py;
  Float_t pz;
  Float_t E;
};

struct three_vector{
  Float_t x;
  Float_t y;
  Float_t z;
};

struct charge_three_vector{
  Float_t cha;
  Float_t x;
  Float_t y;
  Float_t z;
};

#endif
