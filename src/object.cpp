#include "head.h"
#include "object.h"

using namespace std;
void object::subdivide(int sub_level)
{
  num_face=myfaces.size(); num_vertex=myvertexs.size(); num_halfedge=myhalfedges.size();
  this->sub_level=sub_level;
  int i,j,sub_level_now=0;
  while(sub_level_now<sub_level)
    {
      sub_level_now++;
      for(i=0;i<num_face;i++)
	{
	  int index_HE_first=myfaces[i].index_HE;
	  int index_HE_now=myfaces[i].index_HE;
	  myvector fp=myvector(0,0,0);
	  int count=0;
	  do{
	    count++;
	    fp+=myvertexs[myhalfedges[index_HE_now].index_vertex_begin].location;
	    index_HE_now=myhalfedges[index_HE_now].index_nextHE;
	  }while(index_HE_now!=index_HE_first);
	  fp/=(double)count;
	  myfaces[i].fp=fp;
	}
      for(i=0;i<num_halfedge;i++)
	{
	  if(myhalfedges[i].get_ep==0)
	    {
	      myvector ep=myvector(0,0,0);
	      ep+=myfaces[myhalfedges[i].index_face].fp;
	      ep+=myvertexs[myhalfedges[i].index_vertex_begin].location;
	      ep+=myvertexs[myhalfedges[i].index_vertex_towards].location;
	      int index_oppHE=myhalfedges[i].index_oppHE;
	      ep+=myfaces[myhalfedges[index_oppHE].index_face].fp;
	      ep*=0.25;
	      myhalfedges[i].get_ep=1; myhalfedges[i].ep=ep;
	      myhalfedges[index_oppHE].get_ep=1; myhalfedges[index_oppHE].ep=ep;
	    }
	}

      for(i=0;i<num_vertex;i++)
	{	
	  vertex vertex_here=myvertexs[i];
	  int index_HE_begin=vertex_here.index_HE_towards;
	  int index_now=index_HE_begin; //当前的处理的halfedge索引
	  int index_now_opp=myhalfedges[index_now].index_oppHE;
	  int index_face_now=myhalfedges[index_now].index_face;

	  int count=0;
	  myvector help=myvector(0,0,0);
	  do 
	    {	
	      count++;
	      help+=myfaces[index_face_now].fp;
	      help+=myvertexs[myhalfedges[index_now].index_vertex_towards].location;	
	      index_now=myhalfedges[index_now_opp].index_nextHE;
	      index_now_opp=myhalfedges[index_now].index_oppHE;
	      index_face_now=myhalfedges[index_now].index_face;		
	    } while (index_now!=index_HE_begin);
	  myvertexs[i].vp=help/(double)(count*count)+myvertexs[i].location*(double)(count-2)/(double)count;
	}
      vector<vertex > myvertexs_old; vector<face > myfaces_old; vector<halfedge> myhalfedges_old;
      int num_vertex_old=num_vertex; int num_face_old=num_face; int num_halfedge_old=num_halfedge;
      for(i=0;i<num_vertex;i++)
	{
	  myvertexs_old.push_back(myvertexs[i]);
	}
      while(!myvertexs.empty())
	{
	  myvertexs.pop_back();
	}
      for(i=0;i<num_face;i++)
	{
	  myfaces_old.push_back(myfaces[i]);
	}
      while(!myfaces.empty())
	{
	  myfaces.pop_back();
	}
      for(i=0;i<num_halfedge;i++)
	{
	  myhalfedges_old.push_back(myhalfedges[i]);
	}
      while(!myhalfedges.empty())
	{
	  myhalfedges.pop_back();
	}

      //填入新的vertex
      for(i=0;i<num_vertex_old;i++)
	{
	  myvertexs.push_back(vertex(myvertexs_old[i].vp));
	}
      for(i=0;i<num_face_old;i++)
	{
	  myfaces_old[i].index_fp=myvertexs.size();
	  myvertexs.push_back(vertex(myfaces_old[i].fp));
	}
      for(i=0;i<num_halfedge_old;i++)
	{
	  if(myhalfedges_old[i].index_ep==-1)
	    {
	      myhalfedges_old[i].index_ep=myhalfedges_old[myhalfedges_old[i].index_oppHE].index_ep=myvertexs.size();
	      myvertexs.push_back(vertex(myhalfedges_old[i].ep));
	    }
	}
      //构造新的face&halfedge
      map<pair<int,int >,int > mp1,mp2,mp3;
      map<pair<int,int >,int > ::iterator it;
      int index[4]; // 生成的是四边形网格
      int one,two,help;
      int mark;
      int is;

      int num_face_now=0;
      for(i=0;i<num_face_old;i++)
	{
	  int index_HE_first=myfaces_old[i].index_HE;
	  int index_HE_now=myfaces_old[i].index_HE;
	  do{
	    index[0]=myhalfedges_old[index_HE_now].index_vertex_begin;
	    index[1]=myhalfedges_old[index_HE_now].index_ep;
	    index[2]=myfaces_old[i].index_fp;
	    index[3]=myhalfedges_old[myhalfedges_old[index_HE_now].index_prevHE].index_ep;

	    int siz_begin=myhalfedges.size(); //这轮循环开始的size
	    
	    myfaces.push_back(face(siz_begin));
	    for(j=0;j<4;j++)
	      {
		one=index[j%4]; two=index[(j+1)%4]; //当前处理的halfedge的起点和终点
		if(myvertexs[one].index_HE_towards==-1)
		  {
		    myvertexs[one].index_HE_towards=siz_begin+j;
		  }
		else
		  {
		    ;
		  }
			
		if(one<two)
		  {
		    mark=1;
		  }
		else
		  {
		    help=one; one=two; two=help;
		    mark=-1;
		  }
		int siz=myhalfedges.size(); //当前halfedge数目，就是当前处理的halfedge的索引
		if(mp1.find(make_pair(one,two))==mp1.end())
		  {	
		    if(mark==1)
		      {
			mp1[make_pair(one,two)]=1;
			mp2[make_pair(one,two)]=siz; //保存这条halfedge的索引号，待其反向的halfedge出现时建立联系
			myhalfedges.push_back(halfedge(two,one,num_face_now,siz_begin+(j+1)%4,siz_begin+((j-1)%4+4)%4));
		      }
		    else if(mark==-1)
		      {
			mp1[make_pair(one,two)]=-1;  
			mp2[make_pair(one,two)]=siz; //保存这条halfedge的索引号，待其反向的halfedge出现时建立联系
			myhalfedges.push_back(halfedge(one,two,num_face_now,siz_begin+(j+1)%4,siz_begin+((j-1)%4+4)%4));
		      }		
		  }
		else if(mp1[make_pair(one,two)]==1||mp1[make_pair(one,two)]==-1)
		  {
		    int key=mp1[make_pair(one,two)];
		    if(key==1||key==-1)
		      {
			int oppHE=mp2[make_pair(one,two)];
			myhalfedges[oppHE].index_oppHE=siz;			
			if(mark==1)
			  {
			    myhalfedges.push_back(halfedge(two,one,num_face_now,siz_begin+(j+1)%4,siz_begin+((j-1)%4+4)%4,oppHE));
			    mp1[make_pair(one,two)]=2;
			  }
			else if(mark==-1)
			  {
			    myhalfedges.push_back(halfedge(one,two,num_face_now,siz_begin+(j+1)%4,siz_begin+((j-1)%4+4)%4,oppHE));
			    mp1[make_pair(one,two)]=2;  
			  }
		      }
		  }
	      }
	    num_face_now++;
	    index_HE_now=myhalfedges_old[index_HE_now].index_nextHE;
	  }while(index_HE_now!=index_HE_first);
	}
      num_face=myfaces.size(); num_vertex=myvertexs.size(); num_halfedge=myhalfedges.size();
    }
  return ;
}
