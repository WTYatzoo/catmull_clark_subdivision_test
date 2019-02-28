#ifndef _FACE_
#define _FACE_
#include "myvector.h"
class face 
{
 public:
  int index_HE;
  myvector fp;
  int index_fp;
  face(){}
  ~face(){}	
 face(int index_HE):index_HE(index_HE){}
};
#endif
