// 0x68
// функции даты и времени с использованием часов реального времени DS1307, подключенные по I2C. В скетче используется библиотека Wire lib

#include <Wire.h>
#include <RTClib.h>

int hour[2];
int minutes[2];
bool tik = true;

DS1302 RTC;

void setup()
{
    Serial.begin(9600);
    Wire.begin();
    RTC.begin();

    if (!RTC.isrunning())
    {
        Serial.println("RTC is NOT running!");
        // строка ниже используется для настройки даты и времени часов
        // RTC.adjust(DateTime(__DATE__, __TIME__));
    }
}

void loop()
{
      Serial.println(RTC.now());   // выводим время
      delay(1000);  // делаем небольшую задержку, чтобы не выводить время слишком часто 
}
