#ifndef	READH_H
#define	READH_H
#include <string>
#include<map>
#include<vector>

using namespace std;

class ReadH
{
public:

	string Hname;
	string allContent;
	vector<string> HforStruct;

	ReadH(string name);
	~ReadH();

	string readallfile(string hPath);
	void findStruct(string content);

};

#endif