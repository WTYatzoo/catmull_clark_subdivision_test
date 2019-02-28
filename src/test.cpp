#include "head.h"
#include "object.h"
#include "io.h"
using namespace std;

void readcmdline(int argc, char* argv[],boost::property_tree::ptree &para_tree)
{
  int i;
  for(i=1;i<argc;i++)
    {
      string para_here=argv[i];
      size_t pos=para_here.find("=");
      if(pos!= string::npos)
	{
	  string key=para_here.substr(0,pos);
	  string value=para_here.substr(pos+1);
	  para_tree.put(key+".value",value);
	  printf("--[cmdline para] %s %s \n",key.c_str(),value.c_str());
	}
    }
  return;
}
int main(int argc, char *argv[])
{
  // test access point
  boost::property_tree::ptree para_tree;
  readcmdline(argc,argv,para_tree);

  string input_path=para_tree.get<std::string >("input_path.value");
  io myio=io(); object myobj=object();
  myio.getObjData(myobj,input_path);
  int sub_level=para_tree.get<int >("sub_level.value");
  myobj.subdivide(sub_level);
  string output_path=para_tree.get<std::string>("output_path.value");
  myio.saveAsVTK(myobj,output_path);
  
  return 0;
}
