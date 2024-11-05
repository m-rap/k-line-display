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

const int16_t valStartX = 13*textW;
int16_t throttleWidth = 1;
int16_t rpmWidth = 1;
int16_t speedWidth = 1;
int16_t ectWidth = 1;
int16_t maxWidth = 1;

#include "Arduino.h"
#include "OBD9141.h"
#include "AltSoftSerial.h"

#define RX_PIN 0
#define TX_PIN 1

OBD9141 obd;
// https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html
// arduino uno txpin: 9, rxpin: 8
AltSoftSerial altSerial;

uint16_t loopCount = 0;
uint8_t state = 0;

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

  tft.fillRect(valStartX, 0*textH, maxWidth*textW, 4*textH, BLACK);
  uint8_t val = 0;
  tft.setCursor(valStartX, 0*textH);
  tft.print(val);
  tft.setCursor(valStartX, 1*textH);
  tft.print(val);
  tft.setCursor(valStartX, 2*textH);
  tft.print(val);
  tft.setCursor(valStartX, 3*textH);
  tft.print(val);

  tft.fillRect(0, 8*textH, 20*textW, 4*textH, BLACK);
  tft.print("initializing obd...");

  // obd.begin(Serial, RX_PIN, TX_PIN);
  obd.begin(altSerial, RX_PIN, TX_PIN);
}

void measureValWidth(int16_t& maxWidth, int16_t& width, MCUFRIEND_kbv& tft) {
  width = tft.getCursorX() - valStartX;
  if (width > maxWidth) maxWidth = width;
}

void loop(void) {

  if (loopCount > 9999) {
    delay(UINT16_MAX);
    return;
  } else {
    tft.fillRect(valStartX, 6*textH, 3*textW, textH, BLACK);

    tft.setCursor(valStartX, 6*textH);
    tft.print(loopCount);
  }

  if (state == 0) {
    bool res = obd.initKWP();
    
    if (res) {
      state = 1;

      tft.fillRect(0, 8*textH, 20*textW, 4*textH, BLACK);
      tft.print("obd is running...");
    } else {
      delay(1000);
    }
  } else if (state == 1) {
    // tft.fillRect(valStartX, 0*textH, maxWidth*textW, 4*textH, BLACK);
    bool res;

    delay(50);
    res = obd.getCurrentPID(0x11, 1);
    if (res) {
      // throttle
      tft.fillRect(valStartX, 0*textH, throttleWidth*textW, 4*textH, BLACK);
      tft.setCursor(valStartX, 0*textH);
      tft.print(obd.readUint8());
      measureValWidth(maxWidth, throttleWidth, tft);
    }

    delay(50);
    res = obd.getCurrentPID(0x0C, 2);
    if (res) {
      // rpm
      tft.fillRect(valStartX, 0*textH, rpmWidth*textW, 4*textH, BLACK);
      tft.setCursor(valStartX, 1*textH);
      tft.print(obd.readUint8());
      measureValWidth(maxWidth, rpmWidth, tft);
    }

    delay(50);
    res = obd.getCurrentPID(0x0D, 1);
    if (res) {
      // speed
      tft.fillRect(valStartX, 0*textH, speedWidth*textW, 4*textH, BLACK);
      tft.setCursor(valStartX, 2*textH);
      tft.print(obd.readUint8());
      measureValWidth(maxWidth, speedWidth, tft);
    }

    delay(50);
    res = obd.getCurrentPID(0x05, 1);
    if (res) {
      // engine temp
      tft.fillRect(valStartX, 0*textH, ectWidth*textW, 4*textH, BLACK);
      tft.setCursor(valStartX, 3*textH);
      tft.print(obd.readUint8());
      measureValWidth(maxWidth, ectWidth, tft);
    }
  }

  loopCount++;
}
