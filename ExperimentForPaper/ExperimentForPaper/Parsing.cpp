#include"Parsing.h"
#include"ReadH.h"
#include"ReadC.h"
#include<iostream>
using namespace std;

FeatureVector::FeatureVector()
{
}

FeatureVector::~FeatureVector()
{
	entities.clear();
	vector<Function*>(entities).swap(entities);
	features.clear();
	vector<string>(features).swap(features);
}

Parsing::Parsing()
{
	featureVector=new FeatureVector();
}

Parsing::~Parsing()
{
	free(featureVector);
}

void Parsing::parseToEntity(int numberOfC,ReadC *p)
{
	vector<Function*> entities;
	Function* function;
	ReadC *q=p;

	for(int i=0;i<numberOfC;i++)
	{
		map<string,Function*>::iterator iter=q->mapForFunc.begin();
		int j=0;
		for(;iter!=q->mapForFunc.end();iter++)
		{
			function=iter->second;
			function->number=j;
			j++;
			entities.push_back(function);
		}
		q=q->getnext();
	}

	//featureVector->entities=entities;
	for(vector<Function*>::iterator iter=entities.begin();iter!=entities.end();iter++)
		featureVector->entities.push_back(*iter);
}

void Parsing::parseToFeature(int numberOfC,int numberOfH,ReadC *p,map <string, ReadH*> hmap)
{
	vector<string> features;
	string function="";
	string globalVaribles="";
	string userDefinedStruct="";

	ReadC *q=p;
	for(int i=0;i<numberOfC;i++)
	{
		map<string,Function*>::iterator iter=q->mapForFunc.begin();
		for(;iter!=q->mapForFunc.end();iter++)
		{
			function=iter->first;
			if(function!="")
				features.push_back(function);
		}
		q=q->getnext();
	}

	q=p;
	for(int i=0;i<numberOfC;i++)
	{
		map<string,string>::iterator iter=q->mapForGlobal.begin();
		for(;iter!=q->mapForGlobal.end();iter++)
		{
			globalVaribles=iter->first;
			if(globalVaribles!="")
				features.push_back(globalVaribles);
		}
		q=q->getnext();
	}

	q=p;
	for(int i=0;i<numberOfC;i++)
	{
		for(int j=0;j<q->CforStruct.size();j++)
		{
			userDefinedStruct=q->CforStruct.at(j);
			if(userDefinedStruct!="")
				features.push_back(userDefinedStruct);
		}
		q=q->getnext();
	}

	map<string,ReadH*>::iterator iter=hmap.begin();
	ReadH *h;
	for(;iter!=hmap.end();iter++)
	{
		h=iter->second;
		for(int i=0;i<h->HforStruct.size();i++)
		{
			userDefinedStruct=h->HforStruct.at(i);
			if(userDefinedStruct!="")
				features.push_back(userDefinedStruct);
		}
	}

	for(vector<string>::iterator iter1=features.begin();iter1!=features.end();iter1++)
		featureVector->features.push_back(*iter1);
		
	//featureVector->features=features;
}

FeatureVector* Parsing::parseToFeatureVector(int numberOfC,int numberOfH,ReadC *p,map <string, ReadH*> hmap)
{
	parseToEntity(numberOfC,p);
	parseToFeature(numberOfC,numberOfH,p,hmap);
	
	string feature="";
	Function* entity;

	for(int i=0;i<featureVector->entities.size();i++)
	{
		entity=featureVector->entities.at(i);

		for(int j=0;j<featureVector->features.size();j++)
		{
			feature=featureVector->features.at(j);

			string parameters=entity->parameters;
			string returnType=entity->returnType;
			string funcBody=entity->funcBody;

			if((parameters.find(feature)!=string::npos)||
				(returnType.find(feature)!=string::npos)||
				(funcBody.find(feature)!=string::npos))
				featureVector->featureVector[i][j]=1;
			else
				featureVector->featureVector[i][j]=0;
		}
	}

	return featureVector;
	
}