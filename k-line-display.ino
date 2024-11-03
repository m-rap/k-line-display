#include <Adafruit_GFX.h>   // Core graphics library
#include <MCUFRIEND_kbv.h>  // Hardware-specific library
MCUFRIEND_kbv tft;

// #include <Fonts/FreeSans9pt7b.h>
// #include <Fonts/FreeSans12pt7b.h>
// #include <Fonts/FreeSerif12pt7b.h>
// #include <Fonts/FreeMono9pt7b.h>
// #include <Fonts/FreeMonoOblique9pt7b.h>

#include <FreeDefaultFonts.h>

#define BLACK 0x0000
#define RED 0xF800
#define GREEN 0x07E0
#define WHITE 0xFFFF
#define GREY 0x8410

#define textH 16
#define textW 12

#include "Arduino.h"
#include "OBD9141.h"
#include "AltSoftSerial.h"

#define RX_PIN 0
#define TX_PIN 1

OBD9141 obd;
// AltSoftSerial altSerial;

void setup(void) {
  Serial.begin(9600);
  uint16_t ID = tft.readID();
  Serial.println("Example: Font_simple");
  Serial.print("found ID = 0x");
  Serial.println(ID, HEX);
  if (ID == 0xD3D3) ID = 0x9481;  //force ID if write-only display
  tft.begin(ID);
  tft.setRotation(1);
  tft.fillScreen(BLACK);

  tft.setFont(NULL);
  // tft.setTextColor(GREEN);
  tft.setTextSize(2);

  tft.setCursor(0, 0*textH);
  tft.print("Throttle");
  tft.setCursor(0, 1*textH);
  tft.print("RPM");
  tft.setCursor(0, 2*textH);
  tft.print("Speed");
  tft.setCursor(0, 3*textH);
  tft.print("Engine Temp");

  // altSerial.
  obd.begin(Serial, RX_PIN, TX_PIN);
}

void loop(void) {
  // tft.setFont(NULL);
  // tft.setTextColor(GREEN);
  // tft.setTextSize(2);

  // tft.setCursor(0, 0);
  // tft.print("System 1");
  // tft.setCursor(0, 16);
  // tft.print("System 2");
  // tft.setCursor(0, 32);
  // tft.print("System 3");

  bool init_success = obd.initKWP();
  delay(50);

  if (init_success) {
    bool res;
    while (1) {
      res = obd.getCurrentPID(0x11, 1);
      if (res) {
        // Serial.print("Result 0x11 (throttle): ");
        // Serial.println(obd.readUint8());
        tft.setCursor(13*textW, 0*textH);
        tft.print(obd.readUint8());
      }
      delay(50);

      res = obd.getCurrentPID(0x0C, 2);
      if (res) {
        // Serial.print("Result 0x0C (RPM): ");
        // Serial.println(obd.readUint16() / 4);
        tft.setCursor(13*textW, 1*textH);
        tft.print(obd.readUint8());
      }
      delay(50);


      res = obd.getCurrentPID(0x0D, 1);
      if (res) {
        // Serial.print("Result 0x0D (speed): ");
        // Serial.println(obd.readUint8());
        tft.setCursor(13*textW, 2*textH);
        tft.print(obd.readUint8());
      }
      // Serial.println();

      res = obd.getCurrentPID(0x05, 1);
      if (res) {
        // Serial.print("Result 0x05 (eng temp): ");
        // Serial.println(obd.readUint8());
        tft.setCursor(13*textW, 3*textH);
        tft.print(obd.readUint8());
      }

      delay(200);
    }
    // delay(200);
  } else {
    uint8_t val = 0;
    tft.setCursor(13*textW, 0*textH);
    tft.print(val);
    tft.setCursor(13*textW, 1*textH);
    tft.print(val);
    tft.setCursor(13*textW, 2*textH);
    tft.print(val);
    tft.setCursor(13*textW, 3*textH);
    tft.print(val);
  }
  delay(3000);

  // showmsgXY(20, 0, 1, NULL, "System x1");
  // showmsgXY(20, 14, 2, NULL, "System x2");
  // showmsgXY(20, 50, 1, &FreeMono9pt7b, "FreeMono9pt7b");
  // showmsgXY(20, 64, 1, &FreeMonoOblique9pt7b, "FreeMonoOblique9pt7b");

  // showmsgXY(20, 60, 1, &FreeSans9pt7b, "FreeSans9pt7b");
  // showmsgXY(20, 80, 1, &FreeSans12pt7b, "FreeSans12pt7b");
  // showmsgXY(20, 100, 1, &FreeSerif12pt7b, "FreeSerif12pt7b");
  // showmsgXY(20, 120, 1, &FreeSmallFont, "FreeSmallFont");
  // showmsgXY(5, 180, 1, &FreeSevenSegNumFont, "01234");
  // showmsgXY(5, 190, 1, NULL, "System Font is drawn from topline");
  // tft.setTextColor(RED, GREY);
  // tft.setTextSize(2);
  // tft.setCursor(0, 220);
  // tft.print("7x5 can overwrite");
  // delay(1000);
  // tft.setCursor(0, 220);
  // tft.print("if background set");
  // delay(1000);
  // showmsgXY(5, 260, 1, &FreeSans9pt7b, "Free Fonts from baseline");
  // showmsgXY(5, 285, 1, &FreeSans9pt7b, "Free Fonts transparent");
  // delay(1000);
  // showmsgXY(5, 285, 1, &FreeSans9pt7b, "Free Fonts XXX");
  // delay(1000);
  // showmsgXY(5, 310, 1, &FreeSans9pt7b, "erase backgnd with fillRect()");
  // delay(10000);
}

// void showmsgXY(int x, int y, int sz, const GFXfont *f, const char *msg)
// {
//     int16_t x1, y1;
//     uint16_t wid, ht;
//     tft.drawFastHLine(0, y, tft.width(), WHITE);
//     tft.setFont(f);
//     tft.setCursor(x, y);
//     tft.setTextColor(GREEN);
//     tft.setTextSize(sz);
//     tft.print(msg);
//     // delay(1000);
// }
