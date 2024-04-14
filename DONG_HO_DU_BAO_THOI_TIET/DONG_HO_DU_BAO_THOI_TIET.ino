//---------------Thu vien Thoi tiet---------------
#include <ArduinoJson.h>
//#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
WiFiClient wifiClient;
//---------------Thu vien Man hinh TFT-----------
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
//---------------Thu vien Thoi gian--------------
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <SimpleTimer.h>  // Get fron https://github.com/jfturcot/SimpleTimer
//---------------Thu vien Cam bien nhiet do, do am DHT 11 -----------
#include <DHT.h>
#define DHTPIN D7      // what digital pin we're connected to
#define DHTTYPE DHT11  //DHT 11
//Khoi tao DHT sensor.
DHT dht(DHTPIN, DHTTYPE);
//---------------Thu vien doi lich Am - Duong -----------
#include "Date.h"
#include "LunarConverter.h"

//-----------Phan khai bao bien cho mang hinh LCD---------------------------------
//------LCD TFT------ESP8266--------------------
#define TFT_CS D1    //---------------
#define TFT_RST D2   //---------------
#define TFT_DC D3    //---------------
#define TFT_MOSI D4  //---------------
#define TFT_SCLK D5  //---------------
//----------------------------------------------
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
//------------Dinh nghia mau sac----------------
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

static const uint8_t temperature[] = {
  0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x00, 0x4c, 0x80, 0x00, 0x48, 0xc0, 0x18, 0x0a, 0x70, 0x09,
  0xfa, 0x70, 0x03, 0x1a, 0x40, 0x06, 0x0a, 0x70, 0x04, 0x0a, 0xc0, 0x0c, 0x0a, 0x70, 0xec, 0x0a,
  0xc0, 0x04, 0x0a, 0x40, 0x04, 0x13, 0x60, 0x02, 0x17, 0xa0, 0x01, 0xf7, 0x20, 0x08, 0x5a, 0x60,
  0x18, 0x0c, 0xc0, 0x00, 0x47, 0x80, 0x00, 0x40, 0x00, 0x00, 0x40, 0x00
};
static const uint8_t humidity[] = {
  0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x60, 0x00, 0x00, 0xd0, 0x00, 0x01, 0x98, 0x00, 0x03,
  0x0c, 0x00, 0x06, 0x06, 0x00, 0x04, 0x03, 0x00, 0x08, 0x01, 0x00, 0x18, 0x01, 0x80, 0x10, 0x00,
  0x80, 0x10, 0x00, 0xc0, 0x14, 0x00, 0xc0, 0x14, 0x00, 0xc0, 0x16, 0x00, 0x80, 0x1b, 0x00, 0x80,
  0x09, 0xf1, 0x00, 0x0c, 0xf3, 0x00, 0x07, 0x0e, 0x00, 0x01, 0xf8, 0x00
};
const static unsigned char icondo[] = {
  0x00, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0x33, 0x00, 0x33, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char rain[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xe0,
  0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x78, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x1c, 0x00, 0x00,
  0x00, 0x00, 0x0c, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00,
  0x18, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x30, 0x00,
  0x01, 0xf0, 0x00, 0x00, 0x00, 0x20, 0x00, 0x1c, 0x1c, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x30, 0x0e,
  0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x06, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00,
  0x0c, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x08, 0x00,
  0x00, 0x00, 0x00, 0x30, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x08, 0x00, 0x00, 0x00,
  0x00, 0x0c, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x0c,
  0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x03, 0xfc, 0x83, 0xb1, 0x3f, 0xf0, 0x00, 0x00,
  0x00, 0x85, 0x51, 0x00, 0x00, 0x00, 0x00, 0x00, 0x84, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c,
  0x06, 0x00, 0x00, 0x00, 0x00, 0x03, 0x08, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x80, 0x00,
  0x00, 0x00, 0x00, 0x0c, 0x31, 0x88, 0x00, 0x00, 0x00, 0x00, 0x18, 0x63, 0x18, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xc0, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char cloudy[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x03, 0xc3, 0x80, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00,
  0x0c, 0x0f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x0c, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x08, 0x18,
  0x07, 0x00, 0x00, 0x00, 0x00, 0x18, 0x30, 0x03, 0x80, 0x00, 0x00, 0x00, 0x10, 0x60, 0x00, 0x80,
  0x00, 0x00, 0x00, 0x30, 0xc0, 0x00, 0x80, 0x00, 0x00, 0x00, 0x30, 0xc0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x60, 0x80, 0x00, 0x40, 0x00, 0x00, 0x00, 0xc0, 0x80, 0x07, 0xf8, 0x00, 0x00, 0x01, 0x81,
  0x80, 0x0e, 0x0c, 0x00, 0x00, 0x03, 0x03, 0x80, 0x0c, 0x06, 0x00, 0x00, 0x06, 0x06, 0x00, 0x00,
  0x02, 0x00, 0x00, 0x06, 0x0c, 0x00, 0x00, 0x02, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x03, 0x00,
  0x00, 0x04, 0x18, 0x00, 0x00, 0x03, 0x80, 0x00, 0x06, 0x10, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x07,
  0x10, 0x00, 0x00, 0x00, 0x60, 0x00, 0x03, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x03, 0x90, 0x00,
  0x00, 0x00, 0x20, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00,
  0x20, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xc0, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00
};
const unsigned char lighting[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0xf8,
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x06, 0x00, 0x00,
  0x00, 0x00, 0x18, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
  0x30, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x30, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x30, 0x00,
  0x07, 0xf0, 0x00, 0x00, 0x00, 0x30, 0x00, 0x1c, 0x3c, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x30, 0x06,
  0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x03, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
  0x0e, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x08, 0x00,
  0x00, 0x00, 0x00, 0xe0, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x08, 0x00, 0x00, 0x1f,
  0x00, 0x78, 0x00, 0x08, 0x00, 0x00, 0x1f, 0x00, 0x18, 0x00, 0x08, 0x00, 0x00, 0x1b, 0x00, 0x18,
  0x00, 0x0c, 0x00, 0x00, 0x32, 0x00, 0x18, 0x00, 0x0e, 0x00, 0x00, 0x27, 0x00, 0x18, 0x00, 0x07,
  0x80, 0x3e, 0x67, 0x00, 0x38, 0x00, 0x01, 0xfe, 0x66, 0xfb, 0x7f, 0xe0, 0x00, 0x00, 0x3e, 0xcc,
  0xfb, 0x3f, 0x80, 0x00, 0x00, 0x00, 0xce, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x01, 0x82, 0x1e, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x96, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf6, 0x1c, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x3c, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char lighting_rain[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x00, 0xe0, 0x00,
  0x00, 0x00, 0x03, 0x80, 0x00, 0x20, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00,
  0x06, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x3c, 0x00,
  0x00, 0x18, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00,
  0xf0, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x0c, 0x00, 0x0f, 0xfc, 0x00, 0x1c, 0x00,
  0x08, 0x00, 0x18, 0x18, 0x00, 0x0c, 0x00, 0x18, 0x00, 0x18, 0x30, 0x00, 0x0c, 0x00, 0x18, 0x00,
  0x38, 0x70, 0x00, 0x0c, 0x00, 0x0c, 0x00, 0x30, 0xc0, 0x00, 0x0c, 0x00, 0x0c, 0x00, 0x30, 0xff,
  0x00, 0x1c, 0x00, 0x06, 0x00, 0x60, 0x06, 0x00, 0x38, 0x00, 0x03, 0xc0, 0x60, 0x0c, 0x00, 0xf0,
  0x00, 0x01, 0xff, 0x7c, 0x19, 0xff, 0xe0, 0x00, 0x00, 0x1f, 0x04, 0x37, 0xfe, 0x00, 0x00, 0x00,
  0x00, 0x0c, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x69,
  0x87, 0x18, 0x00, 0x00, 0x00, 0x38, 0xcb, 0x0e, 0x30, 0x00, 0x00, 0x00, 0x31, 0x9e, 0x18, 0x60,
  0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x03, 0x8c, 0x41, 0xc7, 0x00, 0x00, 0x00, 0x07, 0x18, 0xe3, 0x8e, 0x00, 0x00, 0x00, 0x0c, 0x31,
  0x87, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char night[] = {
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x10,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x1c,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x1f,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x0f,
  0xc0,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x0f,
  0xc0,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x0e,
  0xe0,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x06,
  0x38,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x06,
  0x38,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x06,
  0x18,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x06,
  0x1c,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x06,
  0x0e,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x06,
  0x06,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x06,
  0x06,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x0e,
  0x06,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x0e,
  0x06,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x1e,
  0x06,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x1c,
  0x06,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x1c,
  0x06,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x38,
  0x06,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x78,
  0x0e,
  0x00,
  0x00,
  0x00,
  0x00,
  0x01,
  0xf0,
  0x0e,
  0x00,
  0x00,
  0x03,
  0x00,
  0x07,
  0xe0,
  0x1c,
  0x00,
  0x00,
  0x01,
  0xff,
  0xff,
  0x80,
  0x1c,
  0x00,
  0x00,
  0x00,
  0xff,
  0xfe,
  0x00,
  0x38,
  0x00,
  0x00,
  0x00,
  0xff,
  0xe0,
  0x00,
  0x78,
  0x00,
  0x00,
  0x00,
  0xf0,
  0x00,
  0x00,
  0xf0,
  0x00,
  0x00,
  0x00,
  0x38,
  0x00,
  0x01,
  0xc0,
  0x00,
  0x00,
  0x00,
  0x3f,
  0x00,
  0x07,
  0xc0,
  0x00,
  0x00,
  0x00,
  0x3f,
  0xc0,
  0x1f,
  0x80,
  0x00,
  0x00,
  0x00,
  0x07,
  0xff,
  0xfe,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0xff,
  0xfc,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x1f,
  0xc0,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
};
const unsigned char clear_sky[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x38, 0x03, 0xc0, 0x1c, 0x00,
  0x00, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00,
  0x3e, 0x0f, 0xf0, 0x78, 0x00, 0x00, 0x00, 0x0c, 0x3f, 0xfc, 0x70, 0x00, 0x00, 0x00, 0x00, 0x7f,
  0xfe, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x3f, 0x80,
  0x00, 0x00, 0x00, 0x03, 0xe0, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x03, 0xc0, 0x00, 0x00,
  0x00, 0x03, 0xc0, 0x03, 0xc0, 0x00, 0x00, 0x07, 0xc7, 0xc0, 0x03, 0xc3, 0xe0, 0x00, 0x0f, 0xc7,
  0xc0, 0x03, 0xe7, 0xe0, 0x00, 0x07, 0xc7, 0xc0, 0x03, 0xe3, 0xe0, 0x00, 0x03, 0xc7, 0xc0, 0x03,
  0xc3, 0xc0, 0x00, 0x00, 0x03, 0xc0, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x07, 0xc0, 0x00,
  0x00, 0x00, 0x03, 0xf0, 0x0f, 0xc0, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00,
  0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x3f, 0xfc, 0x30, 0x00, 0x00, 0x00, 0x1e, 0x1f,
  0xf8, 0x78, 0x00, 0x00, 0x00, 0x3e, 0x03, 0xc0, 0x7c, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x3c,
  0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char clear_cloudy[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x06,
  0x0c, 0x00, 0x00, 0x00, 0x00, 0x01, 0x86, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x18, 0x00,
  0x00, 0x00, 0x00, 0x60, 0xc0, 0x38, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x30, 0x60, 0x00, 0x00,
  0x00, 0x78, 0x3f, 0x80, 0xe0, 0x00, 0x00, 0x00, 0x38, 0x7f, 0xc1, 0xe0, 0x00, 0x00, 0x00, 0x18,
  0xe0, 0xe3, 0xc0, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x73, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xf0, 0x38,
  0x00, 0x00, 0x00, 0x01, 0xf0, 0x78, 0x18, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x3c, 0x18, 0x00, 0x00,
  0x00, 0x03, 0x80, 0x0e, 0x18, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x07, 0x98, 0xf0, 0x00, 0x00, 0x0e,
  0x00, 0x03, 0xfc, 0xf0, 0x00, 0x00, 0x1c, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00, 0x18, 0x00, 0x01,
  0xcf, 0x00, 0x00, 0x00, 0x10, 0x00, 0x03, 0x00, 0xc0, 0x00, 0x00, 0x30, 0x00, 0x02, 0x00, 0xe0,
  0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x70, 0x00, 0x03,
  0xf0, 0x00, 0x00, 0x00, 0x30, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x0e, 0x00, 0x00,
  0x00, 0x00, 0x30, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00,
  0x30, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00,
  0x0c, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x06, 0x00,
  0x00, 0x00, 0x03, 0x80, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff,
  0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char night_cloud[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x3c, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x18, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0xc0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x61, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x81, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x81, 0x00,
  0x00, 0x00, 0x00, 0x03, 0x01, 0x81, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0xc1, 0x80, 0x00, 0x00,
  0x00, 0x0c, 0x00, 0x60, 0xc0, 0x40, 0x00, 0x00, 0x18, 0x00, 0x30, 0x71, 0xc0, 0x00, 0x00, 0x18,
  0x00, 0x3f, 0xbf, 0xc0, 0x00, 0x00, 0x10, 0x00, 0x19, 0xc1, 0x80, 0x00, 0x00, 0x10, 0x00, 0x00,
  0x63, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x3c, 0x00,
  0x00, 0x01, 0x80, 0x00, 0x00, 0x10, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x02,
  0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x06, 0x00, 0x00,
  0x00, 0x02, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x03,
  0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
  0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xe0,
  0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
//-----------------------Phan khai bao cho Thoi gian--------------------------------------------
//--------------Thong tin WiFi ket noi-----
const char *ssid = "172.20.10.12";
const char *password = "1234567899";
//-----------------------------------------
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
//Week Days
String weekDays[7] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
int gio, phut, giay, thu, ngay, thang, nam, ngayam, thangam, namam;
SimpleTimer timer_updateTime;
//-----------------------Phan khai bao cho Thoi tiet----------------------------------------------
//--------------Thoi tiet--------------
String key = "33693a8d53254187b6af4f0a397df91b";
String latitude = "16.075763";    //Kinh do
String longitude = "108.240082";  //Vi do
char server[] = "api.weatherbit.io";
int nhietdo, doam, thoitiet;
String city;
HTTPClient http;

//===================================================================

//=======================Phan SETUP==================================
void setup() {
  Serial.begin(115200);                             //Thiet lap cong noi tiep Serial
  Serial.println("Connecting to " + String(ssid));  //Ket noi vao mang wifi
  WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  // initialize measuring
  pinMode(DHTPIN, INPUT);
  dht.begin();
  setupLCD();                       //Thiet lap Hien thi LCD
  timeClient.begin();               //Khoi chay lay thoi gian tu NTP
  timeClient.setTimeOffset(25200);  //Cai dat mui gio GMT+7

  getThoigian();
  getDHT11Sensor();
  updateTimeLCD();
  //  gio=23;
  //  phut=59;
  //  giay=50;
  timer_updateTime.setInterval(1000, updateTime);
  getWeather();
}
//=========================Phan chuong trinh chinh=========================
void loop() {
  timer_updateTime.run();
  if (giay == 0) {
    getDHT11Sensor();
  }
  if (phut == 0 && giay == 0) {
    Serial.println("Get weather");
    getWeather();
  }
}
//=====================Phan chuong trinh con===============================
void setupLCD() {
  tft.initR(INITR_BLACKTAB);  //Thiet lap LCD TFT
  tft.fillScreen(BLACK);      //Thiet lap mau nen LCD

  tft.drawBitmap(0, 8, temperature, 20, 20, CYAN);  //Hien thi icon nhiet do va do am
  tft.drawBitmap(0, 30, humidity, 20, 20, CYAN);
  getDHT11Sensor();
  tft.drawRect(0, 65, 128, 25, MAGENTA);              //Hien thi khung thoi gian
  tft.drawBitmap(0, 114, temperature, 20, 20, CYAN);  //Hien thi icon nhiet do va do am
  tft.drawBitmap(0, 136, humidity, 20, 20, CYAN);
}
uint32_t dd, mm, yy;
void getThoigian() {
  Serial.println();
  Serial.println("Canculate Lunar -1");

  timeClient.forceUpdate();
  Serial.println();
  Serial.println("Canculate Lunar 0");

  gio = timeClient.getHours();
  phut = timeClient.getMinutes();
  giay = timeClient.getSeconds();
  Serial.println();
  Serial.printf("%02d:%02d:%02d", gio, phut, giay);
  thu = timeClient.getDay();
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime((time_t *)&epochTime);
  ngay = ptm->tm_mday;
  thang = ptm->tm_mon + 1;  //1-->11;
  nam = ptm->tm_year + 1900;
  Serial.println();
  Serial.printf("Thu %0d Ngay %02d Thang %02d Nam %d", thu, ngay, thang, nam);

  Serial.println();
  Serial.println("Canculate Lunar");
  Espace::Date date = Espace::Date(ngay, thang, nam);
  Espace::LunarConverter converter = Espace::LunarConverter();
  Espace::Date lunarDate = converter.Convert(date);
  Serial.println("Canculate Lunar 1");
  ngayam = lunarDate.DayInMonth;
  thangam = lunarDate.Month;
  namam = lunarDate.Year;
  Serial.println("Canculate Lunar 2");
  Serial.println();
  Serial.print(ngayam);
  Serial.print("/");
  Serial.print(thangam);
  Serial.print("/");
  Serial.print(namam);
}
void updateTimeLCD() {
  //Hien thi thu trong tuan
  tft.setCursor(5, 100);
  tft.setTextColor(YELLOW, BLACK);
  tft.setTextSize(1);
  String weekDay = weekDays[thu];
  tft.println(weekDay);

  //Print complete date:
  tft.setCursor(65, 100);
  tft.setTextColor(CYAN, BLACK);
  tft.setTextSize(1);
  String ngay_, thang_;
  if (ngay < 10) {
    ngay_ = "0" + String(ngay);
  } else {
    ngay_ = String(ngay);
  }
  if (thang < 10) {
    thang_ = "0" + String(thang);
  } else {
    thang_ = String(thang);
  }
  String currentDate = ngay_ + "-" + thang_ + "-" + String(nam);
  tft.println(currentDate);
  //Serial.println(currentDate);

  // Display Lunar day
  String ngayam_, thangam_;
  if (ngayam < 10) {
    ngayam_ = "0" + String(ngayam);
  } else {
    ngayam_ = String(ngayam);
  }
  if (thangam < 10) {
    thangam_ = "0" + String(thangam);
  } else {
    thangam_ = String(thangam);
  }
  String dayAm = ngayam_ + "-" + thangam_;
  printText(dayAm, MAGENTA, 65, 37, 2);

  // Display Time

  tft.setCursor(15, 70);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(2);
  String gio_, phut_, giay_;
  if (gio < 10) {
    gio_ = "0" + String(gio);
  } else {
    gio_ = String(gio);
  }
  if (phut < 10) {
    phut_ = "0" + String(phut);
  } else {
    phut_ = String(phut);
  }
  if (giay < 10) {
    giay_ = "0" + String(giay);
  } else {
    giay_ = String(giay);
  }
  String formattedTime = gio_ + ":" + phut_ + ":" + giay_;
  tft.print(formattedTime);
}
void updateTime() {
  //Serial.println("Called updated time");
  giay++;
  if (giay == 60) {
    phut++;
    giay = 0;
  }
  if (phut == 60) {
    gio++;
    phut = 0;
  }
  if (gio == 24) {  // Sau 1 ngày cập nhật thời gian 1 lần

    gio = 0;
    getThoigian();
  }
  updateTimeLCD();
}
void getWeather() {
  http.begin(wifiClient, "http://api.weatherbit.io/v2.0/current?&lat=" + latitude + "&lon=" + longitude + "&key=" + key);
  if (http.GET() == HTTP_CODE_OK) {
    String data = http.getString();
    DynamicJsonBuffer jsonBuffer(1200);
    JsonObject &root = jsonBuffer.parseObject((char *)data.c_str());
    nhietdo = root["data"][0]["temp"].as<int>();
    doam = root["data"][0]["rh"].as<int>();
    thoitiet = root["data"][0]["weather"]["code"].as<int>();
  }
  http.end();
  Serial.println();
  Serial.printf("Nhiệt độ: %d\nĐộ ẩm: %d\nThời tiêt:%d", nhietdo, doam, thoitiet);

  tft.setCursor(25, 116);  //Hien thi nhiet do
  tft.setTextColor(YELLOW, BLACK);
  tft.setTextSize(2);
  tft.print(nhietdo);
  tft.drawBitmap(47, 116, icondo, 10, 20, YELLOW);
  tft.setCursor(58, 116);
  tft.print("C");

  tft.setCursor(25, 140);  //Hien thi do am
  tft.setTextColor(GREEN, BLACK);
  tft.setTextSize(2);
  String do_am = String(doam) + "%";
  tft.print(do_am);

  tft.fillRect(75, 116, 50, 40, BLACK);  //Hien thi icon thoi tiet
  iconthoitiet(75, 116, thoitiet);
}
void getDHT11Sensor() {
  float t = dht.readTemperature();  // Read temperature in *C (default)
  float h = dht.readHumidity();
  Serial.println();
  Serial.print("Teperature:");
  Serial.print((int)round(t));
  Serial.println("*C");
  Serial.print("Humidity:");
  Serial.print((int)round(h));
  Serial.print("%");
  Serial.println();

  tft.setCursor(25, 8);  //Hien thi nhiet do
  tft.setTextColor(YELLOW, BLACK);
  tft.setTextSize(2);
  tft.print((int)round(t));
  tft.drawBitmap(47, 8, icondo, 10, 20, YELLOW);
  tft.setCursor(58, 8);
  tft.print("C");

  printText("AL", MAGENTA, 85, 8, 2);

  tft.setCursor(25, 36);  //Hien thi do am
  tft.setTextColor(GREEN, BLACK);
  tft.setTextSize(2);
  String hst = String((int)round(h)) + "%";
  tft.print(hst);
}
void iconthoitiet(int x, int y, int code) {  //https://www.weatherbit.io/api/codes
  switch (code) {
    // Mua kem sam set
    case 200:
    case 201:
    case 202:
      tft.drawBitmap(x, y, lighting_rain, 50, 40, WHITE);
      break;
    // Sam set
    case 230:
    case 231:
    case 232:
    case 233:
      tft.drawBitmap(x, y, lighting, 50, 40, WHITE);
      break;
    // May + mua
    case 300:
    case 301:
    case 302:
    case 500:
    case 501:
    case 502:
    case 511:
    case 520:
    case 521:
    case 522:
    case 623:
      tft.drawBitmap(x, y, rain, 50, 40, WHITE);
      break;
    // Clear Sky
    case 800:
      if (gio < 18 && gio > 5)
        tft.drawBitmap(x, y, clear_sky, 50, 40, WHITE);
      else
        tft.drawBitmap(x, y, night, 50, 40, WHITE);
      break;
    case 700:
    case 711:
    case 721:
    case 731:
    case 741:
    case 751:
    case 801:
    case 802:
    case 803:
      if (gio < 18 && gio > 5)
        tft.drawBitmap(x, y, clear_cloudy, 50, 40, WHITE);
      else
        tft.drawBitmap(x, y, night_cloud, 50, 40, WHITE);
      break;
    // Trời nhiều mây
    case 611:
    case 612:
    case 804:
      tft.drawBitmap(x, y, cloudy, 50, 40, WHITE);
      break;
  }
}
void printText(String text, uint16_t color, int x, int y, int textSize) {
  tft.setCursor(x, y);
  tft.setTextColor(color, BLACK);  // Background black
  tft.setTextSize(textSize);
  tft.print(text);
}