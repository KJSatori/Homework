#ifndef DECISION_H
#define DECISION_H

#include "Weather.h"

enum PlayType
{
    Play = 0,
    NotPlay = 1
};

class Decision
{
private:
    double percentage;

public:
    Decision(double percentage): percentage(percentage) {}
    ~Decision() {}

    
    PlayType MakeDecision(WeatherType weather);

    double GetPercentage() const;
};

#endif