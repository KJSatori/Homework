#include <iostream>
#include <stdlib.h>
#include "Computing.h"
#include "Experiment.h"
#include <math.h>
using namespace std;

double SingleTest(int weigh, int count)
{
	int* decision;
	decision = new int[count];
	for (int index = 0; index != count; ++index)
	{
		decision[index] = (int)MakeDecision(rand() % 5 + 1, (double)weigh / 10.0);
	}

	double percentage = CompPlayPercentage(decision, count);
	delete[] decision;
	return percentage;
}

void MultipleTest(double* persentageArray, int weigh, int count)
{
	for (int index = 0; index != count ; ++index)
	{
		persentageArray[index] = SingleTest(weigh, 10000);
	}
	return;
}

void DoExperiment(int count)
{
	double** M;
	M = new double* [8];
	int* p;
	p = new int [8] {2, 3, 4, 5, 6, 7, 8, 9};

	for (int index = 0; index != 8; ++index)
	{
		double average, std;
		M[index] = new double[count];
		MultipleTest(M[index], p[index], count);
		CompStatistics(M[index], count, average, std);
		cout << "p = " << p[index] / 10.0 << ": ave = " << average << ", std = " << std << endl;
	}

	for (int index = 0; index != 8; ++index)
	{
		delete[] M[index];
	}
	delete[] M;
	delete[] p;
	return;
}

