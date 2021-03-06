#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <RTClib.h>

#define ONE_WIRE_BUS 2
#define PRIME_RELAY_BUS 12
#define RESERVE_RELAY_BUS 13
#define ON 1
#define OFF 0

// time
RTC_DS1307 RTC;

// temperature
const byte sens[8] = {0x28, 0xFF, 0x70, 0x0F, 0x60, 0x17, 0x04, 0xF9};
OneWire sensDs(ONE_WIRE_BUS);
DallasTemperature ds(&sensDs);

// relay
bool relayStatus = 0;

void setup()
{
    pinMode(PRIME_RELAY_BUS, OUTPUT);
    pinMode(RESERVE_RELAY_BUS, OUTPUT);

    // display
    Wire.begin();
    initDisplay();

    // time
    RTC.begin();
    // if (!RTC.isrunning())
    // {
    //     Serial.begin(9600);
    //     Serial.println("RTC is NOT running!");
    //     // строка ниже используется для настройки даты и времени часов
    //     // RTC.adjust(DateTime(__DATE__, __TIME__));
    // }

    // temperature
    ds.begin();
}

void loop()
{
    ds.requestTemperatures();
    float temperature = ds.getTempC(sens);
    showTemp(temperature);
    isDay() ? dayLogic(temperature) : nightLogic(temperature);
    reserveLogic(temperature);
    delay(60000);
}
