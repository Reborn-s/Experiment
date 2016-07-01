#ifndef	PARSING_H
#define	PARSING_H
/*
对C源码中提取出来的function parse为entity
features:global variables referred to by an entity
		functions called by an entity
		user-defined types accessed by an entity

针对实验源码，选择Jaccard coefficient作为similarity measures
			选择CA作为clustering algorithm
*/
#include"ReadC.h"
#include"ReadH.h"
#include<vector>
using namespace std;


class FeatureVector
{
public:
	vector<Function*> entities;
	vector<string> features;
	char featureVector[10000][10000];

	FeatureVector();
	~FeatureVector();
};

class Parsing
{
public:
	FeatureVector *featureVector;

	Parsing();
	~Parsing();

	void parseToEntity(int numberOfC,ReadC *p);
	void parseToFeature(int numberOfC,int numberOfH,ReadC *p,map <string, ReadH*> hmap);
	FeatureVector *parseToFeatureVector(int numberOfC,int numberOfH,ReadC *p,map <string, ReadH*> hmap);
};

#endif

