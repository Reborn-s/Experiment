#ifndef	CLUSTERINGALGORITHM_H
#define	CLUSTERINGALGORITHM_H
#include"CalculateSimilarity.h"

using namespace std;

class ClusteringAlgorithm
{
public:
	string algorithmName;
	string testcaseName;

	ClusteringAlgorithm();
	virtual ~ClusteringAlgorithm();
	void runAlgorithm();

};

class CAAlorithm:public ClusteringAlgorithm
{
public:
	CAAlorithm(string testcaseName);
	~CAAlorithm();
	void runAlgorithm(FeatureVector* fv);
};

#endif