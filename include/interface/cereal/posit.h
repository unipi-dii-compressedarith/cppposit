#pragma once
#include "../../posit.h"
using namespace posit;

template<class Archive, class T,int totalbits, int esbits, class FT, PositSpec positspec>
void save(Archive & archive, 
          Posit<T,totalbits,esbits,FT,positspec> const & p)
{ 
  archive(float(p)); 
}

template<class Archive, class T,int totalbits, int esbits, class FT, PositSpec positspec>
void load(Archive & archive,
          Posit<T,totalbits,esbits,FT,positspec> & p)
{
  float d;
  archive(d); 
  p = Posit<T,totalbits,esbits,FT,positspec>(d);
}
