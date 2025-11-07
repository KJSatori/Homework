#include <iostream>
#include <ctime>
#include <stdlib.h>
#include "Computing.h"

using namespace std;

bool OneStageDecision(double probability)
{
	return (rand() / (double)RAND_MAX) < probability;
}

bool MakeDecision(int weather, double kenengxin)
{
	// Made by KJS
	if (OneStageDecision(kenengxin)) return false;
	switch (weather)
	{
		case 1:
			return OneStageDecision((kenengxin / 2) * (1 - kenengxin / 2));
			break;
		case 2:
			return OneStageDecision((kenengxin / 2) * kenengxin);
			break;
		case 3:
			return OneStageDecision((1 - kenengxin) * kenengxin);
			break;
		case 4:
			return OneStageDecision((1 - kenengxin) * (1 - kenengxin / 2));
			break;
		case 5:
			return true;
			break;
	}
	return false;
}

double CompPlayPercentage(int* decision, int count)
{
	// duck Professor
	int playCount = 0;
	for(int index = 0; index != count; ++index)
	{
		if (decision[index] == 1) ++playCount;
	}
	return (double)playCount / (double)count;
}

void CompStatistics(double* percentages, int count, double& avehyw, double& stdnum)
{
	// 计算均值
	avehyw = 0.0;
	for (int index = 0; index != count; ++index)
		avehyw += percentages[index];
	avehyw /= count;

	// 计算标准差
	stdnum = 0.0;
	for (int index = 0; index != count; ++index)
		stdnum += (percentages[index] - avehyw) * (percentages[index] - avehyw);
	stdnum = sqrt(stdnum / count);

	return;
}