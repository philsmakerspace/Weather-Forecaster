#ifndef BAROMETERDISPLAY
#define BAROMETERDISPLAY

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define CHANGE_DISPLAY_PERIOD 5000

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "weathercondition.h"

class BarometerDisplay
{

public:
    BarometerDisplay();
    void updateDisplay(float pressure, float temp, WeatherCondition condition);

private:
    Adafruit_SSD1306 *display;
    void drawDial(float pressure);
    void drawPredictionText(String prediction);
    void drawTextCentered(String text, int16_t y);
    String weatherConditionToString(WeatherCondition condition);
    void display1(float pressure, WeatherCondition condition);
    void display2(float pressure, float temp, WeatherCondition condition);
};

#endif