#ifndef Smh_h
#define Smh_h
#include "Arduino.h"

class Smh

{

public:

    void sminit();
    float getTemp();
    float getHumit();
    float getSmoke();
    float getLight();
    int getWater();

    bool getSwitch(int pin);
    bool setDoorSeneor();
    bool getMovement();

    void setLED(int pin, char state[]);
    // ,char[]
    void setLCD(int x, int y, const char[]);
    void setLCD(int x, int y, const int num);
    void setLCD(int x, int y, const float num);

    void ClearText();
    void setBuzzer(char state[]);
    void setSolenoid(char state[]);
    void setRelay(int pin, char state[]);

    bool readCard();
    uint32_t readKeyCard();

    void LedMcpsetup();

};

#endif