#ifndef _VERTEX_
#define _VERTEX_
#include "myvector.h"
class vertex
{
 public:
  myvector location;
  int index_HE_towards; //指向的一条halfedge的索引
  myvector vp;
  vertex(){}
  vertex(myvector location)
    {
      this->location=location;
      index_HE_towards=-1; //-1表示并没有找到一条边由其出发
    }
  ~vertex(){}
};

#endif
