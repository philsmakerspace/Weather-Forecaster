#include "weather.h"

Weather::Weather(BarometerDisplay *display)
{

    if (!bmp.begin(BMP_ADDR))
    {
        Serial.println("Could not find a valid BMP280 sensor, check wiring!");
        while (1)
        {
        };
    }
    _display = display;
}

/// Gets Weather Update
void Weather::getPressureUpdate()
{
    unsigned long now = millis();
    float currentPressure = bmp.readPressure() / 100.0F;
    float currentTemp= bmp.readTemperature();
    if ((_lastSampleTime == 0) || (now - _lastSampleTime >= SAMPLE_PERIOD))
    {

        _pressureBuffer[_bufferIndex] = currentPressure;
    }
    WeatherCondition condition;

    if (!_firstRun)
    {
        condition = getCondition(currentPressure);
    }
    else
    {
        condition = WeatherCondition::NOT_ENOUGH_DATA;
        if (_bufferIndex == BUFFER_SIZE - 1)
        {
            _firstRun = false;
        }
    }
    if ((_lastSampleTime == 0) || (now - _lastSampleTime >= SAMPLE_PERIOD))
    {

        _bufferIndex = (_bufferIndex + 1) % BUFFER_SIZE;
        _lastSampleTime = now;
    }
    _display->updateDisplay(currentPressure,currentTemp, condition);
}

// Gets Condition Based On current pressure against historical pressures over last hour and three hours.
WeatherCondition Weather::getCondition(float currentPressure)
{
    float changeLastHour = getChangeLastHour(currentPressure);
    float changeLastThreeHours = getChangeLastThreeHours(currentPressure);

    if (changeLastHour > 0.03F && changeLastThreeHours > 0.1F)
    {
        return WeatherCondition::CLEARING;
    }
    else if (changeLastHour < -0.03F && changeLastThreeHours < -0.1F)
    {
        return WeatherCondition::DETERIORATING;
    }
    else if (changeLastHour < 0.01F && changeLastThreeHours < 0.03F)
    {
        return WeatherCondition::STABLE_CLOUDY;
    }
    else
    {
        return WeatherCondition::STABLE_CLEAR;
    }
}

// Change Over Last Hour
float Weather::getChangeLastHour(float currentPressure)
{
    int startHourIndex = (_bufferIndex + BUFFER_SIZE - 60) % BUFFER_SIZE;
    float startHourPressure = _pressureBuffer[startHourIndex];
    return currentPressure - startHourPressure;
}

// Average Change Per Minute Over Last Three Hours
float Weather::getChangeLastThreeHours(float currentPressure)
{
    int startThreeHoursIndex = (_bufferIndex + BUFFER_SIZE - 180) % BUFFER_SIZE;
    float startThreeHoursPressure = _pressureBuffer[startThreeHoursIndex];
    return (currentPressure - startThreeHoursPressure) / 180;
}
