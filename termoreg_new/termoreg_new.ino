#include <Wire.h>
#include <OneWire.h>

#define REQUIRESALARMS false
#include <DallasTemperature.h>

#include "GyverTimer.h"
#include <iarduino_RTC.h>
#include "GyverEncoder.h"

#define RELAY_BUS 12
#include "relayControl.h"

#define MIN_TEMP 0
#define MAX_TEMP 30
#include "tuningTemp.h"

#define DISP_CLK 8
#define DISP_DIO 9
#include "displayShow.h"

#define POWER_OFF 0
#define POWER_ON 1
#define POWER_AUTO 2

#define ENC_CLK 2
#define ENC_DT 3
#define ENC_SW 4

#define ONE_WIRE_BUS 10

// timers
GTimer_ms dispTimer(100);
GTimer_ms tempTimer(60000);
GTimer_ms editTimer(10000);
GTimer_ms blinkTimer(2000);

// temperature
const byte sens[8] = {0x28, 0xFF, 0x7F, 0x27, 0x53, 0x17, 0x04, 0xC0};
OneWire sensDs(ONE_WIRE_BUS);
DallasTemperature ds(&sensDs);
float temperature = 0;
int theTemp = 20;

// encoder
Encoder enc1(ENC_CLK, ENC_DT, ENC_SW);
bool editMode = false;
byte power = POWER_AUTO;

boolean isShowTemp = true;

// time
iarduino_RTC time(RTC_DS1302, 5, 7, 6);

void setup()
{
    //Serial.begin(9600);

    Display.init(&theTemp, &temperature);

    // relay
    RC.init(&theTemp, &temperature);

    // tuning
    enc1.setType(TYPE2);
    enc1.setTickMode(AUTO);
    Tuning.init(&theTemp);

    // timers
    editTimer.setMode(MANUAL);

    Wire.begin();
    time.begin();
    ds.begin();
    ds.requestTemperatures();
    delay(1000);
    temperature = ds.getTempC(sens);
    ds.requestTemperatures();
}

void loop()
{
    if (enc1.isTurn())
    {
        editMode = true;
        editTimer.start();
        if (enc1.isRight())
        {
            Tuning.raiseTemp();
        }

        if (enc1.isLeft())
        {
            Tuning.reduceTemp();
        }
        editTimer.reset();
    }
    if (enc1.isPress())
    {
        power = power == 2 ? 0 : power + 1;
        if (power == POWER_ON)
        {
            RC.setRelayOn();
        }
        if (power == POWER_OFF)
        {
            RC.setRelayOff();
        }
        isShowTemp = false;
        blinkTimer.reset();
    }
    if (editTimer.isReady())
    {
        editMode = false;
        time.gettime();
        uint8_t hour = time.Hours;
        RC.setRelay(hour);
        editTimer.reset();
        editTimer.stop();
    }

    if (tempTimer.isReady())
    {
        temperature = ds.getTempC(sens);
        time.gettime();
        uint8_t hour = time.Hours;
        RC.setRelay(hour);
        ds.requestTemperatures();
    }
    if (dispTimer.isReady())
    {
        if (power == POWER_AUTO)
        {
            editMode ? Display.showEdit() : Display.showTemp();
        }
        if (power == POWER_ON)
        {
            isShowTemp ? Display.showTemp() : Display.showOn();
        }
        if (power == POWER_OFF)
        {
            isShowTemp ? Display.showTemp() : Display.showOff();
        }
    }
    if (blinkTimer.isReady())
    {
        isShowTemp = !isShowTemp;
    }
}