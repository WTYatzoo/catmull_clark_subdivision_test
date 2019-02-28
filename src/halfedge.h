#ifndef _HALFEDGE_
#define _HALFEDGE_

#include "halfedge.h"
#include "myvector.h"

class halfedge
{
 public:

  int index_vertex_towards;
  int index_vertex_begin;
  int index_face;
  int index_nextHE;
  int index_prevHE;
  int index_oppHE;

  myvector ep;
  int get_ep; //是否已经获得ep
  int index_ep;
  ~halfedge(){}

  
 halfedge(int index_vertex_towards,int index_vertex_begin,int index_face,int index_nextHE,int index_prevHE,int index_oppHE):index_vertex_towards(index_vertex_towards),index_vertex_begin(index_vertex_begin),index_face(index_face),index_nextHE(index_nextHE),index_prevHE(index_prevHE),index_oppHE(index_oppHE)
  {
    get_ep=0;
    index_ep=-1;
  } //对于一条边的第二条halfedge，因为第一条halfedge已经得到，所以反向边初始化即可得到
	
	
  halfedge(){
    get_ep=0;
    index_ep=-1;
  }

 halfedge(int index_vertex_towards,int index_vertex_begin,int index_face,int index_nextHE,int index_prevHE):index_vertex_towards(index_vertex_towards),index_vertex_begin(index_vertex_begin),index_face(index_face),index_nextHE(index_nextHE),index_prevHE(index_prevHE){
    get_ep=0;
    index_ep=-1;
  } //对于一条边的第一条halfedge，无法预知第二条halfedge索引，故初始化时不管他

};
#endif
