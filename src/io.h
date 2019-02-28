#ifndef _IO_
#define _IO_
#include "head.h"
#include "object.h"
class io
{
 public:
  io(){}
  ~io(){}
  void saveAsVTK(object &myobj,std::string output_path);
  void getObjData(object &myobj,std::string input_path);
};
#endif
