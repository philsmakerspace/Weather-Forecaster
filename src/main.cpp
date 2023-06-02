#include "Wire.h"
#include "Arduino.h"
#include "barometerdisplay.h"
#include "weather.h"

BarometerDisplay *display;
Weather *weather;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin(SDA, SCL);
  display = new BarometerDisplay();
  weather = new Weather(display);
}

void loop()
{
  weather->getPressureUpdate();
}