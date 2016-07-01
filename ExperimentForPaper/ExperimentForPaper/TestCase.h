#ifndef	TESTCASE_H
#define	TESTCASE_H

#include<string>
#include<vector>
#include"ReadC.h"
#include"ReadH.h"
#include"Parsing.h"
#ifdef WIN32
#include "Shlwapi.h"
#include <direct.h>
#include <io.h>
#else 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <sys/time.h>
#define sprintf_s sprintf
#endif

using namespace std;
class TestCase
{
public:
	string casePath;					// ..//testcases//CVS
	string testCaseMenu;
	string testCaseName;
	ReadC *head;
	vector < string >	aryC_fileName;
	vector < string >	aryH_fileName;
	map <string, ReadH*> hmap;
	FeatureVector *fv;

	#ifdef WIN32
	DWORD	runtime;
	#else
	timeval	runtime;
	#endif
	string  writeTime;
	int		allTime;

	TestCase();
	~TestCase();

	void dfsFolder(string path);

	void readEachFile();

	void doReading();

	void doParsing();

	void doClustering();

};

#endif