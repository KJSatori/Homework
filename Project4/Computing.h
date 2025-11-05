#pragma once
void PrintResults(int* weatherPointer, bool* resultPointer, int count);

bool OneStageDecision(double probability);

/// <summary>
/// 根据天气和概率做出决策
/// </summary>
/// <param name="weather">
/// 1代表Sunny and Humid
///	2代表Sunny and not Humid
/// 3代表Raining and Windy
/// 4代表Raining and not Windy
///	5代表Overcast
/// </param>
/// <param name="probability"></param>
/// <returns></returns>
bool MakeDecision(int weather, double probability);

double CompPlayPercentage(int* decision, int count);

void CompStatistics(double* percentages, int count, double& ave, double& std);