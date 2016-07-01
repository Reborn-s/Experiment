// ExperimentForPaper.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"TestCase.h"
#include<iostream>

using namespace std;


int main(int argc, _TCHAR* argv[])
{
	cout<<"start!"<<endl;
	TestCase *t=new TestCase();
	t->doClustering();
	return 0;
}


