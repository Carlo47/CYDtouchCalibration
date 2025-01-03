/**
 * Program      CYDtouchCalibration
 * 
 * Author       2025-01-03 Charles Geiser (www.dodeka.ch)
 * 
 * Purpose      Shows how to calibrate the touchpad of the CYD
 * 
 * Board        ESP32-2432S028
 * 
 * Remarks      Uses a XPT2046_Bitbang library extended by me, originally 
 *              published by Claus Näveke. The touch routines of LovyanGFX 
 *              are not used. Do not forget to comment out the corresponding 
 *              section in the header file lgfx_ESP32_2432S028.h.
 * 
 * References   https://github.com/lovyan03/LovyanGFX
 *              https://registry.platformio.org/libraries/nitek/XPT2046_Bitbang_Slim (software SPI)
 * 
*/

#include <Arduino.h>
#include <SD.h>
#include "lgfx_ESP32_2432S028.h"
#include "XPT2046_Bitbang.h"

using Action = void(&)(LGFX &lcd);

extern void nop(LGFX &lcd);
extern void grid(LGFX &lcd);
extern void grid(LGFX &lcd, int w=TFT_WIDTH, int h=TFT_HEIGHT, int d=20);
extern GFXfont defaultFont;
extern void initDisplay(LGFX &lcd, uint8_t rotation=0, GFXfont *theFont=&defaultFont, Action greet=nop);
extern void initSDCard(SPIClass &spi);
extern void framedCrosshair(LGFX &lcd);
extern bool saveBMPtoSD_24bit(LGFX &lcd, const char *filename);

enum class ROTATION { LANDSCAPE_USB_RIGHT, PORTRAIT_USB_UP, LANDSCAPE_USB_LEFT, PORTRAIT_USB_DOWN };

LGFX lcd;
XPT2046_Bitbang touchpad(lcd, TP_MOSI, TP_MISO, TP_SCLK, TP_CS);
SPIClass sdcardSPI(VSPI);

// These points are used for calibration when called with useCalibrationPoints()
TouchPoint calibrationPoints[] = {{ 90,  50, 0,0,0},   // upper left point
                                  {290, 210, 0,0,0}};  // lower right point

void checkTouchpadCalibration()
{
  int x,y;
  bool done = false;

  while (! done)
  {
    if (touchpad.isCalibrationDataAvailable())
    {
      touchpad.recallCalibrationData();
      touchpad.printCalibrationData();
      lcd.setCursor(30, 20);  lcd.print(" Touchpad is calibrated ");
      lcd.setCursor(30, 80);  lcd.print(" Recalibrate? ");             //40..160,90
      lcd.setCursor(30, 100); lcd.print(" Clear calibration data? ");  //40..260,110
      lcd.setCursor(30, 120); lcd.print(" Clear prefs and restart? "); //40..270,130
      lcd.setCursor(30, 140); lcd.print(" Continue? ");                //40..150,150
      while (! touchpad.getTouch(x, y)) delay(100);
      vTaskDelay(pdMS_TO_TICKS(500));
      if     (touchpad.touchedAt(x, y, 100,  90,  60, 10)) touchpad.useCalibrationPoints(calibrationPoints, 5);
      else if(touchpad.touchedAt(x, y, 150, 110, 110, 10)) touchpad.clearCalibrationData();
      else if(touchpad.touchedAt(x, y, 155, 130, 115, 10)) touchpad.erasePreferences();
      else if(touchpad.touchedAt(x, y,  95, 150,  55, 10)) {lcd.clear(); done = true; }
      else if(touchpad.touchedAt(x, y, 60,60,5,5)) saveBMPtoSD_24bit(lcd, "/calibrated.bmp");
    }
    else
    {
      lcd.setCursor(30, 20);  lcd.print(" Touchpad is not calibrated ");
      lcd.setCursor(30, 80);  lcd.print(" Calibrate? ");                // 40..140,90
      lcd.setCursor(30, 100); lcd.print(" Use programmed defaults? ");  // 40..300,110
      while (! touchpad.getTouch(x, y)) delay(100);
      vTaskDelay(pdMS_TO_TICKS(500));
      if     (touchpad.touchedAt(x, y,  90,  90,  50, 10)) touchpad.useCalibrationPoints(calibrationPoints, 5);
      else if(touchpad.touchedAt(x, y, 170, 110, 130, 10)) touchpad.saveCalibrationData();
      else if(touchpad.touchedAt(x, y, 60,60,5,5)) saveBMPtoSD_24bit(lcd, "/uncalibrated.bmp");
    }
    touchpad.printCalibrationData(); 
  } 
}


void setup() 
{
  Serial.begin(115200);
  initDisplay(lcd, static_cast<uint8_t>(ROTATION::LANDSCAPE_USB_RIGHT), &defaultFont, grid);
  initSDCard(sdcardSPI);
  touchpad.begin();
  checkTouchpadCalibration();
  lcd.clear();
  grid(lcd, lcd.width(), lcd.height()-39, 20);
}


void loop() 
{
  TouchPoint tpoint;

  if (touchpad.getTouch(tpoint))
  {
    lcd.fillRect(0, 205, 320, 40, TFT_BLACK);
    lcd.setCursor(10,220);
    lcd.printf("x = %d, y = %d", tpoint.x, tpoint.y);
    log_i("x / y = %d / %d  xValue / yValue = %d / %d", tpoint.x, tpoint.y, tpoint.xValue, tpoint.yValue);
    delay(200);
  }   
}
