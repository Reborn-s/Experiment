#include"CalculateSimilarity.h"

using namespace std;

CalculateSimilarity::CalculateSimilarity()
{
	similarity=0.0;
}

CalculateSimilarity::~CalculateSimilarity()
{
}

double CalculateSimilarity::calculate(double a,double b,double c)
{
	//similarity=a/(a+b+c);
	if(a+b+c==0.0)
		return 0.0;
	else
		return a/(a+b+c);
}

