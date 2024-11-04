#include <Adafruit_GFX.h>   // Core graphics library
#include <MCUFRIEND_kbv.h>  // Hardware-specific library
MCUFRIEND_kbv tft;

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
// https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html
// arduino uno txpin: 9, rxpin: 8
AltSoftSerial altSerial;
bool initSuccess;

uint16_t loopCount = 0;

void setup(void) {
  uint16_t ID = tft.readID();
  if (ID == 0xD3D3) ID = 0x9481;  //force ID if write-only display
  tft.begin(ID);
  tft.setRotation(1);
  tft.fillScreen(BLACK);

  tft.setFont(NULL);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);

  tft.setCursor(0, 0*textH);
  tft.print("Throttle");
  tft.setCursor(0, 1*textH);
  tft.print("RPM");
  tft.setCursor(0, 2*textH);
  tft.print("Speed");
  tft.setCursor(0, 3*textH);
  tft.print("Engine Temp");
  tft.setCursor(0, 6*textH);
  tft.print("Loop Count");

  uint8_t val = 0;
  tft.setCursor(13*textW, 0*textH);
  tft.print(val);
  tft.setCursor(13*textW, 1*textH);
  tft.print(val);
  tft.setCursor(13*textW, 2*textH);
  tft.print(val);
  tft.setCursor(13*textW, 3*textH);
  tft.print(val);

  // obd.begin(Serial, RX_PIN, TX_PIN);
  obd.begin(altSerial, RX_PIN, TX_PIN);

  initSuccess = false;
}

void loop(void) {

  if (loopCount > 3) {
    tft.fillRect(13*textW, 0*textH, 5*textW, 4*textH, BLACK);
    return;
  } else {
    tft.fillRect(13*textW, 6*textH, 3*textW, 1*textH, BLACK);

    tft.setCursor(13*textW, 6*textH);
    tft.print(loopCount);
  }

  if (!initSuccess) {
    initSuccess = obd.initKWP();
    delay(50);
  }
  
  if (!initSuccess) {
    tft.fillRect(13*textW, 0*textH, 5*textW, 4*textH, BLACK);
    uint8_t val = 0;
    tft.setCursor(13*textW, 0*textH);
    tft.print(val);
    tft.setCursor(13*textW, 1*textH);
    tft.print(val);
    tft.setCursor(13*textW, 2*textH);
    tft.print(val);
    tft.setCursor(13*textW, 3*textH);
    tft.print(val);

    delay(3000);
  } else {
    bool res;

    res = obd.getCurrentPID(0x11, 1);
    if (res) {
      // throttle
      tft.setCursor(13*textW, 0*textH);
      tft.print(obd.readUint8());
    }
    delay(50);

    res = obd.getCurrentPID(0x0C, 2);
    if (res) {
      // rpm
      tft.setCursor(13*textW, 1*textH);
      tft.print(obd.readUint8());
    }
    delay(50);

    res = obd.getCurrentPID(0x0D, 1);
    if (res) {
      // speed
      tft.setCursor(13*textW, 2*textH);
      tft.print(obd.readUint8());
    }
    delay(50);

    res = obd.getCurrentPID(0x05, 1);
    if (res) {
      // engine temp
      tft.setCursor(13*textW, 3*textH);
      tft.print(obd.readUint8());
    }

    // delay(200);
    delay(50);
  }

  loopCount++;
}
