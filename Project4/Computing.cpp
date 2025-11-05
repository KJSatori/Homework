#include <iostream>
#include <ctime>
#include <stdlib.h>
#include "Computing.h"

using namespace std;

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