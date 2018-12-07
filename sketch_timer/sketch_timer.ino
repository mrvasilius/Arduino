// 0x68
// функции даты и времени с использованием часов реального времени DS1307, подключенные по I2C. В скетче используется библиотека Wire lib

#include <Wire.h>
#include <RTClib.h>

int hour[2];
int minutes[2];
bool tik = true;

//массив изображений 7 сегментных символов от 0 до 9 и пустота
byte simv[11] = {
    B00111111, //0
    B00000110, //1
    B01011011, //2
    B01001111, //3
    B01100110, //4
    B01101101, //5
    B01111101, //6
    B00000111, //7
    B01111111, //8
    B01101111, //9
    B00000000, //пустота
};

byte dot = B10000000;

RTC_DS1307 RTC;

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

    // адреса регистров управления и данных ТМ1650
    // 1-0х24
    // 2-0х25
    // 3-0х26
    // 4-0х27
    // *********инициализация дисплея************
    //___________________________________________
    //b7 b6 b5 b4 b3 b2 b1 b0 IФункция   I опис. I
    //___________________________________________
    //x  0  0  0      x  x    I          I   8   I
    //x  0  0  1      x  x    I          I   1   I
    //x  0  1  0      x  x    I          I   2   I
    //x  0  1  1      x  x    I яркость  I   3   I
    //x  1  0  0      x  x    I          I   4   I
    //x  1  0  1      x  x    I          I   5   I
    //x  1  1  0      x  x    I          I   6   I
    //x  1  1  1      x  x    I          I   7   I
    //____________________________________________
    //x           0   x  x    I  режим   I 8ceгм I
    //x           1   x  x    I 7/8 bit  I 7ceгм I
    //____________________________________________
    //x               x  x  0 Ibit ON/OFFI  OFF  I
    //x               x  x  1 I  дисплея I   ON  I
    //____________________________________________

    Wire.beginTransmission(0x24); //обращаемся к любому порту 0х24-0х27
    Wire.write(0b01000001);       //включаем:яркость 4,8 битный режим, отображение ON
    Wire.endTransmission();       // закрываем сессию
}

void loop()
{
    DateTime now = RTC.now();
    uint8_t hh = now.hour();
    uint8_t mm = now.minute();
    if (hh < 10)
    {
        hour[0] = 0;
    }
    else
    {
        hour[0] = 1;
    }
    hour[1] = (hh % 10);

    if (mm < 10)
    {
        minutes[0] = 0;
    }
    else
    {
        minutes[0] = (mm / 10);
    }
    minutes[1] = (mm % 10);

    Wire.beginTransmission(0x34); //Пишем в 1 разряд
    Wire.write(simv[hour[0]]);    //
    Wire.endTransmission();       // закрываем сессию

    Wire.beginTransmission(0x35); //Пишем вo 2 разряд
    tik ? Wire.write(simv[hour[1]]) : Wire.write(simv[hour[1]] ^ dot);
    Wire.endTransmission(); // закрываем сессию

    Wire.beginTransmission(0x36); //Пишем в 3 разряд
    Wire.write(simv[minutes[0]]);
    Wire.endTransmission(); // закрываем сессию

    Wire.beginTransmission(0x37); //Пишем в 4 разряд
    Wire.write(simv[minutes[1]]);
    Wire.endTransmission(); // закрываем сессию

    
    tik = !tik;
    delay(1000);
}
