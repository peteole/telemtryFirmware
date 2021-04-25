#include <Arduino.h>

#define leftLED PB8
#define rightLED PB9
long nextBlinkTrigger = 0;
byte blinkStatus = 0;
void startBlink()
{
    analogWrite(PA6, 255);
    analogWrite(PA7, 255);
    nextBlinkTrigger = millis() + 20;
    blinkStatus++;
}
void endBlink()
{
    analogWrite(PA6, 0);
    analogWrite(PA7, 0);
    if (blinkStatus == 1)
    {
        nextBlinkTrigger = millis() + 100;
        blinkStatus++;
    }
    else
    {
        nextBlinkTrigger = millis() + 1000;
        blinkStatus = 0;
    }
}
void updateBlinkStatus()
{
    if (millis() > nextBlinkTrigger)
    {
        if (blinkStatus % 2 == 0)
        {
            startBlink();
        }
        else
        {
            endBlink();
        }
    }
}
void setLED(int power)
{
    digitalWrite(PC13, power < 100 ? HIGH : LOW);
    analogWrite(rightLED, power);
    analogWrite(leftLED, power);
}