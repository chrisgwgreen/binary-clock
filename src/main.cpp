#define PIN D4

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <WiFiManager.h>
#include <NTPClient.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

const unsigned int MAXLEDS = 16;
Adafruit_NeoPixel *pixels;
uint8_t colorOrder = NEO_GRBW;

const char *ssid = "Breath";

void drawHour(byte aByte)
{
  int i = 0;
  for (int8_t aBit = 7; aBit >= 0; aBit--)
  {
    Serial.write(bitRead(aByte, aBit) ? '1' : '0');
    pixels->setPixelColor(i,
                          bitRead(aByte, aBit) ? pixels->Color(82, 204, 206) : pixels->Color(0, 0, 0));
    i++;
  }

  Serial.println("");
  pixels->show();
}

void drawMinute(byte aByte)
{
  int i = 15;
  for (int8_t aBit = 7; aBit >= 0; aBit--)
  {
    Serial.write(bitRead(aByte, aBit) ? '1' : '0');
    pixels->setPixelColor(i,
                          bitRead(aByte, aBit) ? pixels->Color(255, 56, 100) : pixels->Color(0, 0, 0));
    i--;
  }

  Serial.println("");
  pixels->show();
}

void getNetworkTime()
{
  timeClient.update();

  int currentHour = timeClient.getHours();
  Serial.print("Hour: ");
  Serial.println(currentHour);

  drawHour(currentHour);

  int currentMinute = timeClient.getMinutes();
  Serial.print("Minutes: ");
  Serial.println(currentMinute);

  drawMinute(currentMinute);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Setting up...");

  Serial.println("Configuring pixels...");
  pixels = new Adafruit_NeoPixel(MAXLEDS, PIN, colorOrder);
  pixels->begin();
  pixels->setBrightness(3);

  WiFiManager wm;

  // wm.resetSettings();
  // ESP.eraseConfig();

  bool res;
  res = wm.autoConnect("Binary Clock");

  if (!res)
    Serial.println("Failed to connect");

  Serial.println();
  Serial.println("Setup complete!");
}

void loop()
{
  getNetworkTime();
  delay(5000);
}