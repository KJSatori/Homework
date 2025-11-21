#include "Weather.h"
#include <stdlib.h>
#include <time.h>
#include "Utils.h"
#include <iostream>
using namespace std;

Weather::Weather()
{
    int randomInt = rand() % 5 + 1;
    current = (WeatherType)randomInt;
}

/// @brief 在对应地方填入不同天气出现的概率，并返回随机天气类型
/// @param pSunnyAndHumid 
/// @param pSunnyAndNotHumid 
/// @param pRainingAndWindy 
/// @param pRainingAndNotWindy 
/// @param pOvercast 
/// @return 天气类型
WeatherType Weather::RandWeather(const double& pSunnyAndHumid, 
    const double pSunnyAndNotHumid,
    const double pRainingAndWindy,
    const double pRainingAndNotWindy,
    const double pOvercast
) const
{
    double randomDouble = GenerateRandf();

    double percentages[5] = {0};
    percentages[0] = pSunnyAndHumid;
    percentages[1] = pSunnyAndNotHumid + percentages[0];
    percentages[2] = pRainingAndWindy + percentages[1];
    percentages[3] = pRainingAndNotWindy + percentages[2];
    percentages[4] = pOvercast + percentages[3];

    if(0 <= randomDouble && randomDouble < percentages[0])
        return WeatherType::SunnyAndHumid;
    else if(percentages[0] <= randomDouble && randomDouble < percentages[1])
        return WeatherType::SunnyAndNotHumid;
    else if(percentages[1] <= randomDouble && randomDouble < percentages[2])
        return WeatherType::RainingAndWindy;
    else if(percentages[2] <= randomDouble && randomDouble < percentages[3])
        return WeatherType::RainingAndNotWindy;
    else if(percentages[3] <= randomDouble && randomDouble <= percentages[4])
        return WeatherType::Overcast;

    cerr << "Warning: RandWeather中随机有问题"<< 
        " pSunnyAndHumid: "<< pSunnyAndHumid <<
        " pSunnyAndNotHumid: " << pSunnyAndNotHumid <<
        " pRainingAndWindy: " << pRainingAndWindy <<
        " pRainingAndNotWindy: " << pRainingAndNotWindy <<
        " pOvercast: " << pOvercast << endl;

    return WeatherType::None;
}

WeatherType Weather::GetWeather() const
{
    return current;
}

void Weather::UpdateWeather()
{
    switch (current)
    {
    case WeatherType::SunnyAndHumid:
        current = RandWeather(0.1, 0.3, 0.2, 0, 0.4);
        break;
    case WeatherType::SunnyAndNotHumid:
        current = RandWeather(0.3, 0.2, 0.1, 0.1, 0.3);
        break;
    case WeatherType::RainingAndWindy:
        current = RandWeather(0.2, 0.1, 0.1, 0.3, 0.3);
        break;
    case WeatherType::RainingAndNotWindy:
        current = RandWeather(0.3, 0.1, 0.2, 0.2, 0.2);
        break;
    case WeatherType::Overcast:
        current = RandWeather(0.2, 0.2, 0.2, 0.2, 0.2);
        break;
    }
}

void Weather::PrintWeather() const
{
    cout << "当前天气为: " << current << endl;
}