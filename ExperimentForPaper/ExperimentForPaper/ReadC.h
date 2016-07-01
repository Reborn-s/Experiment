#ifndef READC_H
#define READC_H
#include<string>
#include<vector>
#include<map>

#include"Function.h"
using namespace std;


class ReadC
{
public:
	string Cname;
	string allContent;
	string strForGlobal;
	ReadC *next;
	vector<string> CforStruct;		//structName,structBody
	map<string,Function*> mapForFunc;	//funcName,function
	map<string,string> mapForGlobal;	//globalName,filename

	ReadC(string name);
	~ReadC();

	void getGlobalArea(string content);
	bool isFunction(string content,int pos);
	string removeComment(string content);
	int* findRight(string content,int pos,int tright,int lastBodyEnd);
	Function* dealWithOldC(string content,int pos,int left,int right);
	Function* dealWithNewC(string content,int pos,int left,int right);
	void findFunctions(string content,string name);
	void findStruct(string content);
	void findGlobal(string content,string name);			//寻找.c里的全局变量，实际上是所有{}之外的字符串内容
	string readallfile(string cPath);
	ReadC* getnext();
	void setnext(ReadC *p);

};

#endif