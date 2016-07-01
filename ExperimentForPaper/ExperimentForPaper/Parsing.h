#ifndef	PARSING_H
#define	PARSING_H
/*
��CԴ������ȡ������function parseΪentity
features:global variables referred to by an entity
		functions called by an entity
		user-defined types accessed by an entity

���ʵ��Դ�룬ѡ��Jaccard coefficient��Ϊsimilarity measures
			ѡ��CA��Ϊclustering algorithm
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

