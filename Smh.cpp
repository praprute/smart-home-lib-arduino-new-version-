
#include "Smh.h"
#include <Adafruit_Sensor.h>
#include "DHT.h"
#include "Wire.h"
#include <LiquidCrystal_I2C.h>
#include "Adafruit_MCP23017.h"
#include <SPI.h>
#include <RFID.h>
#define led_pin1 8
#define led_pin2 9
#define led_pin3 10

#define btn_pin1 11
#define btn_pin2 12
#define btn_pin3 13
#define btn_pin4 14

#define MQ2 33
#define BUZZER 27
#define SOLENOID 12
#define DOORSENSOR 13
#define LDR 35
#define PIR 34

#define RELAY1 25
#define RELAY2 26

#define WATER_LEAK 39

#define SS_PIN 4
#define RST_PIN 2

#define DHTPIN 32
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
Adafruit_MCP23017 mcp;
// LiquidCrystal_I2C lcd(0x27, 16, 2);
LiquidCrystal_I2C lcd(0x27, 16, 2);
RFID rfid(SS_PIN, RST_PIN);
// RFID rfid(SS_PIN, RST_PIN);

int serNum0;
int serNum1;
int serNum2;
int serNum3;
int serNum4;

int led_pin[3] = {led_pin1, led_pin2, led_pin3};
int btn_pin[4] = {btn_pin1, btn_pin2, btn_pin3, btn_pin4};
int relay[2] = {RELAY1, RELAY2};
int pirVal = 0;

void Smh::sminit()
{
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(DOORSENSOR, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(RELAY1, 1);
  digitalWrite(RELAY2, 1);
  dht.begin();
  lcd.begin();
  lcd.backlight();
  mcp.begin();
  Serial.begin(9600);
  pinMode(PIR, INPUT);
  mcp.pinMode(8, OUTPUT);
  mcp.pinMode(9, OUTPUT);
  mcp.pinMode(10, OUTPUT);
  SPI.begin();
  rfid.init();
}

bool Smh::readCard()
{
  if (rfid.isCard())
  {
    return true;
  }
  else
  {
    return false;
  }
}
uint32_t Smh::readKeyCard()
{
  if (rfid.readCardSerial())
  {
    uint32_t serNum0 = (rfid.serNum[0]);
    uint32_t serNum1 = (rfid.serNum[1]);
    uint32_t serNum2 = (rfid.serNum[2]);
    uint32_t serNum3 = (rfid.serNum[3]);
    uint32_t key = (serNum0 << 24) + (serNum1 << 16) + (serNum2 << 8) + (serNum3);
    return key;
  }
  rfid.halt();
}

void Smh::setLCD(int x, int y, const char c[])
{
  x = x - 1;
  y = y - 1;
  lcd.setCursor(x, y);
  lcd.print(c);
}

void Smh::setLCD(int x, int y, const int num)
{
  x = x - 1;
  y = y - 1;
  lcd.setCursor(x, y);
  lcd.print(num);
}

void Smh::setLCD(int x, int y, const float num)
{
  x = x - 1;
  y = y - 1;
  lcd.setCursor(x, y);
  lcd.print(num);
}

void Smh::ClearText()
{
  lcd.clear();
}

bool getON(char state[])
{
  if (state == "ON")
  {
    return HIGH;
  }
  else if (state == "OFF")
  {
    return LOW;
  }
}

void Smh::setLED(int pin, char state[])
{
  mcp.digitalWrite(led_pin[pin - 1], getON(state));
}

bool Smh::getSwitch(int pin)
{
  mcp.pullUp(pin, HIGH);
  delay(20);
  if (mcp.digitalRead(btn_pin[pin - 1]) == 1)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void Smh::setRelay(int pin, char state[])
{
  digitalWrite(relay[pin - 1], !getON(state));
}

int Smh::getWater()
{
  Serial.println("getWater true");
  int Readwater = analogRead(WATER_LEAK);
  delay(100);
  if (Readwater <= 300)
  {
    Serial.println("with out water");
  }
  else
  {
    Serial.println("water");
  }
  return Readwater;
}

bool Smh::getMovement()
{
  pirVal = digitalRead(PIR);
  if (pirVal == 0)
  {
    return false;
  }
  else
  {
    return true;
  }
}

float Smh::getLight()
{
  float light = analogRead(LDR);
  light = light / 10;
  return light;
}

bool Smh::setDoorSeneor()
{
  // Serial.println("setDoorSeneor true");
  int state;
  state = digitalRead(DOORSENSOR);
  if (state == HIGH)
  {
    return true;
  }
  else
  {
    return false;
  }
}

float Smh::getSmoke() //success
{
  float mq = analogRead(MQ2);
  return mq;
}

float Smh::getTemp()
{
  float t = dht.readTemperature();
  return t;
}

float Smh::getHumit()
{
  float h = dht.readHumidity();
  return h;
}

void Smh::setBuzzer(char state[])
{
  digitalWrite(BUZZER, getON(state));
}

void Smh::setSolenoid(char state[])
{
  digitalWrite(SOLENOID, getON(state));
}
