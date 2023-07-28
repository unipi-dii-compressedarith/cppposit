#pragma once
#include "../../posit.h"
using namespace posit;
//#include "../../posit12.hpp"
#include "../../tabulated/posit10.hpp"
#include "../../tabulated/posit8.hpp"

/*template<class Archive, class T,int totalbits, int esbits, class FT, PositSpec positspec>
void save(Archive & archive, 
          Posit<T,totalbits,esbits,FT,positspec> const & p)
{ 
  archive(p.v); 
}

template<class Archive, class T,int totalbits, int esbits, class FT, PositSpec positspec>
void load(Archive & archive,
          Posit<T,totalbits,esbits,FT,positspec> & p)
{
  T d;
  archive(d); 
  p.v = d;
  float a = (float)p;
} */

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

template<class Archive>
void save(Archive & archive, 
          posit10 const & p)
{ 
  archive(float(p)); 
}

template<class Archive>
void load(Archive & archive,
          posit10 & p)
{
  float d;
  archive(d); 
  p = posit10(d);
}

template<class Archive>
void save(Archive & archive, 
          posit8 const & p)
{ 
  archive(float(p)); 
}

template<class Archive>
void load(Archive & archive,
          posit8 & p)
{
  float d;
  archive(d); 
  p = posit8(d);
}  

/*template<class Archive>
void save(Archive & archive, 
          posit14 const & p)
{ 
  archive(float(p)); 
}

template<class Archive>
void load(Archive & archive,
          posit14 & p)
{
  float d;
  archive(d); 
  p = posit14(d);
} */

/*template<class Archive>
void save(Archive & archive, 
          posit12 const & p)
{ 
  archive(float(p)); 
}

template<class Archive>
void load(Archive & archive,
          posit12 & p)
{
  float d;
  archive(d); 
  p = posit12(d);
}*/ 
