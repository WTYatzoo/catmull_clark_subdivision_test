#include "io.h"
using namespace std;

void io::saveAsVTK(object &myobj,std::string output_path)
{
  FILE *fp;
  int i,j;
  fp=fopen(output_path.c_str(),"w");
  fprintf(fp,"# vtk DataFile Version 2.0\n");
  fprintf(fp,"quad\n");
  fprintf(fp,"ASCII\n");
  fprintf(fp,"DATASET UNSTRUCTURED_GRID\n");
  fprintf(fp,"POINTS %d double\n",myobj.num_vertex);
  for(i=0;i<myobj.num_vertex;i++)
    {
      fprintf(fp,"%lf %lf %lf\n",myobj.myvertexs[i].location.x,myobj.myvertexs[i].location.y,myobj.myvertexs[i].location.z);
    }
  int num_help;
  if(myobj.sub_level==0) // just triangle 
    {
      num_help=myobj.help;
    } 
  else // become quad
    {
      num_help=myobj.help;
    }
  fprintf(fp,"CELLS %d %d\n",myobj.num_face,myobj.num_face*(num_help+1));
  for(i=0;i<myobj.num_face;i++)
    {
      fprintf(fp,"%d",num_help); 
      halfedge HE_now=myobj.myhalfedges[myobj.myfaces[i].index_HE];
      for(j=0;j<num_help;j++)
	{
	  fprintf(fp," %d",HE_now.index_vertex_begin);
	  HE_now=myobj.myhalfedges[HE_now.index_nextHE];
	}
      fprintf(fp,"\n");
    }
  fprintf(fp,"CELL_TYPES %d\n",myobj.num_face);

  int pp;
  if(num_help==3)
    {
      pp=5;
    }
  else if(num_help==4)
    {
      pp=9;
    }
  for(i=0;i<myobj.num_face;i++)
    {
      
      fprintf(fp,"%d\n",pp);
    }
  fclose(fp);
}

void io::getObjData(object &myobj,std::string input_path)
{
  int help;
  FILE* file=fopen(input_path.c_str(),"r");
  fscanf(file,"%d%d%d",&myobj.num_vertex,&myobj.num_face,&help);
  printf("%d %d %d\n",myobj.num_vertex,myobj.num_face,help);

  myobj.help=help;
  int i,j;
  double x,y,z;
  vertex here;
  char  filter[5];
  int num_vertex=myobj.num_vertex;
  int num_face=myobj.num_face;
  while(!myobj.myvertexs.empty())
    {
      myobj.myvertexs.pop_back();
    }
  while(!myobj.myfaces.empty())
    {
      myobj.myfaces.pop_back();
    }
  while(!myobj.myhalfedges.empty())
    {
      myobj.myhalfedges.pop_back();
    }
  
  for(i=0;i<num_vertex;i++)
    {     
      fscanf(file,"%s%lf%lf%lf",filter,&x,&y,&z);
      myobj.myvertexs.push_back(vertex(myvector(x,y,z)));
    }
	
  map<pair<int,int >,int > mp1,mp2,mp3;
  map<pair<int,int >,int > ::iterator it;
  int index[4]; //输入为三角网格或者四边形网格
  int one,two,help_change;
  int mark;

  for(i=0;i<num_face;i++)
    {
      fscanf(file,"%s",filter);
      for(j=0;j<help;j++)
	{
	  fscanf(file,"%d",&index[j]);
	  index[j]--;
	}
      int siz_begin=myobj.myhalfedges.size(); //这轮循环开始的size
      myobj.myfaces.push_back(face(siz_begin));
      for(j=0;j<help;j++)
	{
	  one=index[j%help]; two=index[(j+1)%help]; //当前处理的halfedge的起点和终点
	  if(myobj.myvertexs[one].index_HE_towards==-1)
	    {
	      myobj.myvertexs[one].index_HE_towards=siz_begin+j;
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
	      help_change=one; one=two; two=help_change;
	      mark=-1;
	    }
	  int siz=myobj.myhalfedges.size(); //当前halfedge数目，就是当前处理的halfedge的索引
	  if(mp1.find(make_pair(one,two))==mp1.end())
	    {	
	      if(mark==1)
		{
		  mp1[make_pair(one,two)]=1;
		  mp2[make_pair(one,two)]=siz; //保存这条halfedge的索引号，待其反向的halfedge出现时建立联系
		  myobj.myhalfedges.push_back(halfedge(two,one,i,siz_begin+(j+1)%help,siz_begin+((j-1)%help+help)%help));
                    
		}
	      else if(mark==-1)
		{
		  mp1[make_pair(one,two)]=-1;  
		  mp2[make_pair(one,two)]=siz; //保存这条halfedge的索引号，待其反向的halfedge出现时建立联系
		  myobj.myhalfedges.push_back(halfedge(one,two,i,siz_begin+(j+1)%help,siz_begin+((j-1)%help+help)%help));
		}
				
	    }
	  else if(mp1[make_pair(one,two)]==1||mp1[make_pair(one,two)]==-1)
	    {
	      int key=mp1[make_pair(one,two)];
	      if(key==1||key==-1)
		{
		  int oppHE=mp2[make_pair(one,two)];
		  myobj.myhalfedges[oppHE].index_oppHE=siz;
					
		  if(mark==1)
		    {
		      myobj.myhalfedges.push_back(halfedge(two,one,i,siz_begin+(j+1)%help,siz_begin+((j-1)%help+help)%help,oppHE));
		      mp1[make_pair(one,two)]=2;
		    }
		  else if(mark==-1)
		    {
		      myobj.myhalfedges.push_back(halfedge(one,two,i,siz_begin+(j+1)%help,siz_begin+((j-1)%help+help)%help,oppHE));
		      mp1[make_pair(one,two)]=2;  
		    }
		}
	    }
	}		
    }
}
