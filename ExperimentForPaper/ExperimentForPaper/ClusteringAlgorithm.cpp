#include"ClusteringAlgorithm.h"
#include<iostream>
#include <algorithm>
#include<fstream>
using namespace std;

ClusteringAlgorithm::ClusteringAlgorithm()
{
}

ClusteringAlgorithm::~ClusteringAlgorithm()
{
}

void ClusteringAlgorithm::runAlgorithm()
{
}

CAAlorithm::CAAlorithm(string name)
{
	algorithmName="CA";
	testcaseName=name;
}

CAAlorithm::~CAAlorithm()
{
}

typedef pair<string,double> PAIR;
int cmp(const PAIR& x,const PAIR& y)
{
	return x.second>y.second;			
}

void CAAlorithm::runAlgorithm(FeatureVector* fv)
{
	int clusterCount=0;
	int iterations=0;
	int clusterCountIterations[10000];
	map<int, int> clusterSize;
	map<int, map<int,int>> clusterSizeIterations;
	
	map<string,double> simiMap;
	vector<Function*> entity=fv->entities;
	vector<string> features=fv->features;
	
	char **featureVector;
	featureVector= (char**)malloc(sizeof(Function)*entity.size());
	for(int m=0;m<entity.size();m++)
		featureVector[m]=(char*)malloc(sizeof(string)*features.size());
	
	cout<<sizeof(*featureVector)<<endl;
	for(int i=0;i<entity.size();i++)
	{
		for(int k=0;k<features.size();k++)
		{
			featureVector[i][k]=fv->featureVector[i][k];
		}
	}

	double similarity=0.0;
	char pair_p[10];
	char pair_q[10];
	double a=0.0;
	double b=0.0;
	double c=0.0;
	do{
		int isbreak=0;
		for(int f=0;f<entity.size();f++)
		{
			if(entity.at(f)->isClustered==true)
				isbreak++;
		}
		if(clusterCount==1&&isbreak==entity.size())
			break;

		for(int i=0;i<entity.size()-1;i++)
		{	
			for(int j=i+1;j<entity.size();j++)
			{
				a=0.0;
				b=0.0;
				c=0.0;
				char whichpair[20]="";
				
				for(int k=0;k<features.size();k++)
				{
					if(featureVector[i][k]==1&&featureVector[j][k]==1)
						a++;
					if(featureVector[i][k]==1&&featureVector[j][k]==0)
						b++;
					if(featureVector[i][k]==0&&featureVector[j][k]==1)
						c++;
				}
				similarity=CalculateSimilarity::calculate(a,b,c);
				itoa(i,whichpair,10);
				itoa(i,pair_p,10);
				itoa(j,pair_q,10);
				whichpair[strlen(pair_p)]='s';
				strcat(whichpair,pair_q);
				simiMap.insert(pair<string,double>(whichpair,similarity));
			}
		}

		vector<PAIR> simiVector(simiMap.begin(),simiMap.end());
		sort(simiVector.begin(),simiVector.end(),cmp);
		
		string clusterPair=simiVector.at(0).first;
		/*
		for(int r=0;r<50;r++)
			cout<<simiVector.at(r).first<<" "<<simiVector.at(r).second<<endl;
		*/
		int spos=clusterPair.find('s');
		int p=atoi(clusterPair.substr(0,spos).c_str());		//p和q需要聚成clutser
		int q=atoi(clusterPair.substr(spos+1,clusterPair.length()-1-spos).c_str());
		
		if(entity.at(p)->isClustered==false&&entity.at(q)->isClustered==false)
		{
			clusterCount++;
			entity.at(p)->clusterNum=clusterCount-1;
			entity.at(q)->clusterNum=clusterCount-1;
			fv->entities.at(entity.at(p)->number)->clusterNum=clusterCount-1;
			fv->entities.at(entity.at(q)->number)->clusterNum=clusterCount-1;
		}
		if(entity.at(p)->isClustered==true&&entity.at(q)->isClustered==true)
		{
			clusterCount--;
			for(int loop=0;loop<fv->entities.size();loop++)
				if(fv->entities[loop]->clusterNum==entity.at(q)->clusterNum)
					fv->entities[loop]->clusterNum=entity.at(p)->clusterNum;

		}
		if(entity.at(p)->isClustered==true&&entity.at(q)->isClustered==false)
			fv->entities.at(entity.at(q)->number)->clusterNum=entity.at(p)->clusterNum;
		if(entity.at(p)->isClustered==false&&entity.at(q)->isClustered==true)
			fv->entities.at(entity.at(p)->number)->clusterNum=entity.at(q)->clusterNum;

		fv->entities.at(entity.at(p)->number)->isClustered=true;
		fv->entities.at(entity.at(q)->number)->isClustered=true;

		//为newly formed cluster赋予新的feature
		for(int k=0;k<features.size();k++)
			featureVector[p][k]=(fv->featureVector[p][k])|(fv->featureVector[q][k]);
		
		for(int d=q;d<entity.size()-1;d++)
		{
			for(int k=0;k<features.size();k++)
				featureVector[d][k]=featureVector[d+1][k];
		}


		entity.at(p)->isClustered=true;
		
		entity.erase(entity.begin()+q);
		simiMap.clear();

		clusterCountIterations[iterations]=clusterCount;
		cout<<"iterations:"<<iterations<<"	"<<"clusterCount:"<<clusterCount<<endl;
		for(int i=0;i<fv->entities.size();i++)
		{
			int part=fv->entities.at(i)->clusterNum;
			if(part!=-1)
			{
				map<int, int>::iterator iter=clusterSize.find(part);
				if(iter==clusterSize.end())
					clusterSize[part]=1;
				else
					clusterSize[part]++;
			}
		}
		clusterSizeIterations[iterations]=clusterSize;
		clusterSize.clear();
		iterations++;

	}while(clusterCount>=1&&entity.size()>=1);

	cout<<"Iterations:"<<iterations<<endl;
	for(int i=0;i<iterations;i++)
	{
		cout<<i<<"	"<<clusterCountIterations[i]<<endl;
	}

	fstream result;					
	result.open("..//output/"+testcaseName+".txt",ios::out);
	if(!result)                        
    {
        cout<<"error open"<<endl;
        exit(0);    
    }
	for(int i=0;i<iterations;i++)
	{
		result<<i<<"	"<<clusterCountIterations[i]<<"\n";
	}
	for(int j=0;j<iterations;j++)
	{
		result<<j<<"\n";
		map<int,int> clusterSize=clusterSizeIterations[j];
		map<int,int>::iterator iter=clusterSize.begin();
		for(;iter!=clusterSize.end();iter++)
			result<<iter->second<<" ";
		result<<"\n";
	}
	result.close();

	for(int m=0;m<entity.size();m++)
		free(featureVector[m]);
	free(featureVector);

}