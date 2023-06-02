#ifndef WEATHER_H
#define WEATHER_H

#include "barometerdisplay.h"
#include "weathercondition.h"

#define BMP_ADDR 0x76

#define SAMPLE_PERIOD 60000           // Sample every minute
#define PRESSURE_CHANGE_THRESHOLD 1.0 // Change threshold in hPa to consider as significant
#define BUFFER_SIZE 180               // Store readings for the last hour


#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>


class Weather
{

public:
    Weather(BarometerDisplay *display);
    void getPressureUpdate();

private:
    Adafruit_BMP280 bmp;
    float _pressureBuffer[BUFFER_SIZE];
    int _bufferIndex = 0;
    bool _firstRun=true;
    BarometerDisplay *_display;
    unsigned long _lastSampleTime = 0;
    float getChangeLastHour(float currentPressure);
    float getChangeLastThreeHours(float currentPressure);
    WeatherCondition getCondition(float currentPressure);

    
};

#endif