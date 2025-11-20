#include "Decision.h"
#include "Weather.h"
#include "Utils.h"
#include <iostream>
using namespace std;

PlayType Decision::MakeDecision(WeatherType weather)
{
    if (GenerateRandf() < 1 - percentage)
    {
        switch (weather)
        {
        case WeatherType::SunnyAndHumid:
            if(GenerateRandf() < (percentage / 2) * percentage) 
                return PlayType::Play;
            break;
        case WeatherType::SunnyAndNotHumid:
            if(GenerateRandf() < (percentage / 2) * (1 - percentage / 2)) 
                return PlayType::Play;
            break;
        case WeatherType::RainingAndWindy:
            if(GenerateRandf() < (1 - percentage) * percentage) 
                return PlayType::Play;
            break;
        case WeatherType::RainingAndNotWindy:
            if(GenerateRandf() < (1 - percentage) * (1 - percentage / 2)) 
                return PlayType::Play;
            break;
        case WeatherType::Overcast:
            return PlayType::Play;
            break;
        case WeatherType::None:
            cerr << "Error: WeatherType为None" << endl;
            break;
        }
    }
    return PlayType::NotPlay;
}

double Decision::GetPercentage() const
{
    return percentage;
}