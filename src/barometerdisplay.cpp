#include "barometerdisplay.h"

BarometerDisplay::BarometerDisplay()
{
    display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT);
    Serial.println("Setup Display");
    if (!display->begin(SSD1306_SWITCHCAPVCC, 0x3c))
    {
        Serial.println("SSD1306 allocation failed");
        while (1)
            ;
    }
    delay(2000);
    display->clearDisplay();
    display->display();
}

String BarometerDisplay::weatherConditionToString(WeatherCondition condition)
{
    switch (condition)
    {
    case WeatherCondition::STABLE:
        return "Weather is stable";
    case WeatherCondition::CLEARING:
        return "Weather is improving";
    case WeatherCondition::DETERIORATING:
        return "Bad weather coming";
    case WeatherCondition::STABLE_CLEAR:
        return "Stable and clear";
    case WeatherCondition::STABLE_CLOUDY:
        return "Stable but cloudy";
    case WeatherCondition::NOT_ENOUGH_DATA:
        return "Not enough data";
    default:
        return "Unknown weather condition";
    }
}

void BarometerDisplay::drawPredictionText(String prediction)
{
    int16_t x1, y1;
    uint16_t w, h;
    display->setTextSize(1);
    display->setTextColor(SSD1306_WHITE);

    // Calculate the width and height of the text
    display->getTextBounds(prediction, 0, 0, &x1, &y1, &w, &h);

    // Calculate x position to start the string to make it appear centered
    int x = (SCREEN_WIDTH - w) / 2;

    display->setCursor(x, SCREEN_HEIGHT - 10);
    display->println(prediction);
}

void BarometerDisplay::drawDial(float pressure)
{
    int centerX = SCREEN_WIDTH / 2;
    int centerY = (SCREEN_HEIGHT / 2) - 5;
    int radius = min(centerX, centerY) - 4;

    // Draw dial
    display->drawCircle(centerX, centerY, radius, SSD1306_WHITE);

    // Draw needle
    float angle = map(pressure, 950, 1050, -120, 120); // Normalize pressure to angle (-120 to 120 degrees)
    angle = radians(angle);                            // Convert to radians
    int needleX = centerX + radius * sin(angle);
    int needleY = centerY - radius * cos(angle);
    display->drawLine(centerX, centerY, needleX, needleY, SSD1306_WHITE);

    // Draw labels
    display->setTextSize(1);
    display->setTextColor(SSD1306_WHITE);
    display->setCursor(centerX - radius - 25, centerY - 5);
    display->println("950");
    display->setCursor(centerX + radius + 7, centerY - 5);
    display->println("1050");
}

// A function to draw centered text at a specific y position
void BarometerDisplay::drawTextCentered(String text, int16_t y)
{
    int16_t x1, y1;
    uint16_t w, h;
    display->setTextSize(1);
    display->setTextColor(SSD1306_WHITE);

    // Calculate the width and height of the text
    display->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

    // Calculate x position to start the string to make it appear centered
    int x = (SCREEN_WIDTH - w) / 2;

    display->setCursor(x, y);
    display->println(text);
}

void BarometerDisplay::display1(float pressure, WeatherCondition condition)
{
    drawDial(pressure);
    drawPredictionText(weatherConditionToString(condition));
}

void BarometerDisplay::display2(float pressure, float temp, WeatherCondition condition)
{
    String pressureStr = "Pressure: " + String(pressure) + "hPa";
    String tempStr = "Temperature: " + String(temp) + "c";
    String conditionStr = weatherConditionToString(condition);

    drawTextCentered(pressureStr, SCREEN_HEIGHT / 3);      // Place at 1/3 height
    drawTextCentered(tempStr, SCREEN_HEIGHT / 2);          // Place at half height
    drawTextCentered(conditionStr, 2 * SCREEN_HEIGHT / 3); // Place at 2/3 height
}

void BarometerDisplay::updateDisplay(float pressure, float temp, WeatherCondition condition)
{
    static unsigned long lastDisplaySwitch = 0;
    static bool displaySwitch = false;

    if (millis() - lastDisplaySwitch > 10000) // Check if 10 seconds have passed
    {
        lastDisplaySwitch = millis();   // Update the last display switch time
        displaySwitch = !displaySwitch; // Switch the display
    }

    display->clearDisplay();

    // Depending on the value of displaySwitch, call the appropriate display function
    if (displaySwitch)
    {
        display1(pressure, condition);
    }
    else
    {
        display2(pressure, temp, condition);
    }

    display->display();
}