#include <iostream>
#include <ctime>
#include <stdlib.h>
#include "Computing.h"

using namespace std;

void PrintResults(int* weatherPointer, bool* resultPointer, int count)
{
	for (int index = 0; index != count; ++index)
	{
		cout << index + 1 << ".Weather:\t";
		switch (weatherPointer[index])
		{
		case 1:
			cout << "Sunny and Humid" << endl;
			break;
		case 2:
			cout << "Sunny and not Humid" << endl;
			break;
		case 3:
			cout << "Raining and Windy" << endl;
			break;
		case 4:
			cout << "Raining and not Windy" << endl;
			break;
		case 5:
			cout << "Overcast" << endl;
			break;
		}
		if (index + 1 >= 10)
		{
			cout << " ";
		}
		cout << "  Decision:\t";
		if (resultPointer[index] == 0)
		{
			cout << "Not Play" << endl;
		}
		else
		{
			cout << "Play" << endl;
		}
		cout << endl;
	}
	return;
}

bool OneStageDecision(double probability)
{
	return (rand() / (double)RAND_MAX) < probability;
}

bool MakeDecision(int weather, double probability)
{
	if (OneStageDecision(probability)) return false;
	switch (weather)
	{
		case 1:
			return OneStageDecision((probability / 2) * (1 - probability / 2));
			break;
		case 2:
			return OneStageDecision((probability / 2) * probability);
			break;
		case 3:
			return OneStageDecision((1 - probability) * probability);
			break;
		case 4:
			return OneStageDecision((1 - probability) * (1 - probability / 2));
			break;
		case 5:
			return true;
			break;
	}
	return false;
}

double CompPlayPercentage(int* decision, int count)
{
	int playCount = 0;
	for(int index = 0; index != count; ++index)
	{
		if (decision[index] == 1) ++playCount;
	}
	return (double)playCount / (double)count;
}

void CompStatistics(double* percentages, int count, double& ave, double& std)
{
	// 计算均值
	ave = 0.0;
	for (int index = 0; index != count; ++index)
		ave += percentages[index];
	ave /= count;

	// 计算标准差
	std = 0.0;
	for (int index = 0; index != count; ++index)
		std += (percentages[index] - ave) * (percentages[index] - ave);
	std = sqrt(std / count);

	return;
}