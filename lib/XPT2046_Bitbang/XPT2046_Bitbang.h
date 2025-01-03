/**
 * Header       XPT2046_Bitbang.h
 * 
 * Author       2024-12-24 Charles Geiser (https://www.dodeka.ch)
 *              It is an extension of the library by Claus Näveke nitek/XPT2046_Bitbang_Slim
 * 
 * Purpose      Declaration of the class XPT2046_Bitbang
 * 
 * Constructor
 * arguments    &lcd        reference to LGFX display
 *              mosiPin     ESP32 pin MOSI
 *              misoPin     ESP32 pin MISO
 *              clkPin      ESP32 pin CLK
 *              csPin       ESP32 pin CS
 * 
 * References   https://registry.platformio.org/libraries/nitek/XPT2046_Bitbang_Slim
 *              https://github.com/lovyan03/LovyanGFX
 */

#pragma once
#include <Arduino.h>
#include <nvs_flash.h>
#include <Preferences.h>
#include <LovyanGFX.hpp>
#include "lgfx_ESP32_2432S028.h"

#define CMD_READ_X   0x91 // Command for XPT2046 to read X position
#define CMD_READ_Y   0xD1 // Command for XPT2046 to read Y position
#define CMD_READ_Z1  0xB1 // Command for XPT2046 to read Z1 position
#define CMD_READ_Z2  0xC1 // Command for XPT2046 to read Z2 position

#define DELAY 5

using Callback = void (*)(int x, int y);

using TouchPoint = struct tpnt
{
    int x,       y,               // screen coordinates
        xValue,  yValue, zValue;  // raw values read from touchpad
};

using TouchCalibration = struct tcal
{
    TouchPoint touchMin; 
    TouchPoint touchMax;
};


class XPT2046_Bitbang 
{
    public:
        XPT2046_Bitbang(LGFX &lcd, uint8_t mosiPin, uint8_t misoPin, uint8_t clkPin, uint8_t csPin);
        void begin();
        void loop();
        bool getTouch();
        bool getTouch(TouchPoint& tp);
        bool getTouch(int &xScreen, int &yScreen);
        void useCalibrationPoints(TouchPoint tp[], int nbrTouches);
        bool isCalibrationDataAvailable();
        void clearCalibrationData();
        void saveCalibrationData();
        void erasePreferences();
        bool recallCalibrationData();
        void printCalibrationData();
        bool touchedAt(int x, int y, int x0, int y0, int dx, int dy);

        void addShortTouchCb(Callback cb);
        void addLongTouchCb(Callback cb);
        void addSwipeLeftCb(Callback cb);
        void addSwipeRightCb(Callback cb);
        void addSwipeUpCb(Callback cb);
        void addSwipeDownCb(Callback cb);

    private:
        LGFX&   _lcd;
        uint8_t _rotation;
        uint8_t _mosiPin;
        uint8_t _misoPin;
        uint8_t _clkPin;
        uint8_t _csPin;
        uint32_t _msPenDown;
        uint32_t _msPenUp;
        uint32_t _msTouchDuration;
        const uint32_t long _msLongTouchMinDuration  = 280UL;
        const uint32_t long _msShortTouchMinDuration = 35UL;
        TouchPoint _tp;
        TouchPoint _tpPenDown;
        TouchPoint _tpPenUp;
        const int _minSwipeDiff = 20;
        int _swipeDir;
        TouchCalibration _cal;
        int      _getSwipeDir(TouchPoint tpPenDown, TouchPoint tpPenUp);
        void     _writeSPI(byte command);
        uint16_t _readSPI(byte command);
        Preferences _prefs;

        Callback _onShortTouch = nullptr;
        Callback _onLongTouch  = nullptr;
        Callback _onSwipeLeft  = nullptr;
        Callback _onSwipeRight = nullptr;
        Callback _onSwipeUp    = nullptr;
        Callback _onSwipeDown  = nullptr;
        void _crosshair(TouchPoint p, int s, uint16_t color);
};

