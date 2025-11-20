#ifndef WEATHER_H
#define WEATHER_H

enum WeatherType
{
    None = 0,
    SunnyAndHumid = 1,
    SunnyAndNotHumid = 2,
    RainingAndWindy = 3,
    RainingAndNotWindy = 4,
    Overcast = 5
};

class Weather
{
private:
    WeatherType current;
    WeatherType RandWeather(const double& pSunnyAndHumid, 
        const double pSunnyAndNotHumid,
        const double pRainingAndWindy,
        const double pRainingAndNotWindy,
        const double pOvercast
    ) const;

public:
    Weather(WeatherType weather): current(weather) {}
    Weather();
    ~Weather() {}
    WeatherType GetWeather() const;

    void UpdateWeather();

    //调试函数
    void PrintWeather() const;
};

#endif