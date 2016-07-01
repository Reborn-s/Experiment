#include<map>
#include"Function.h"

using namespace std;

Parameter::Parameter(string type,string name)
{
	ptype=type;
	pname=name;
}

Function::Function()
{
	isClustered=false;
	clusterNum=-1;
	isClustered=false;
}