#ifndef	CALCULATESIMILARITY_H
#define	CALCULATESIMILARITY_H
#include"Parsing.h"

using namespace std;

class CalculateSimilarity
{
public:
	double similarity;
	double static calculate(double a,double b,double c);	//formula

	CalculateSimilarity();
	~CalculateSimilarity();
};

#endif

