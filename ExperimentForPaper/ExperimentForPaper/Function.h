#include<string>
#include<vector>
using namespace std;

class Parameter
{
public:
	string pname;
	string ptype;

	Parameter(string type,string name);
};

class Function
{
public:
	string funcName;
	string parameters;
	string returnType;
	string funcBody;
	string InCFile;
	bool isClustered;
	int clusterNum;
	bool isIgnored;
	int number;


	Function();
	~Function();


};