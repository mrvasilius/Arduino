#include <Wire.h>
#include <OneWire.h>
#include <RTClib.h>

#define ONE_WIRE_BUS 2
#define RELAY_BUS 3

OneWire  sensDs(ONE_WIRE_BUS);
RTC_DS1307 RTC;





void setup() {
  Wire.begin();
  RTC.begin();
  Serial.begin(9600);

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

void loop() {
  float temp;
  temp = getTemp();
  showTemp(temp);
//  if (temp < 22) {
//    digitalWrite(RELAY_BUS, HIGH);
//  } else {
//    digitalWrite(RELAY_BUS, LOW);    
//  }
//  Serial.println(getTemp());
//  Serial.println(' ');
delay(60000);
}
