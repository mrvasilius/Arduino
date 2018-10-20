#include <Wire.h>  
// библиотека для LCD I2C: 
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
#include <LiquidCrystal_I2C.h>
//  LCD адрес - 0x27 для 20 символьного 4 строкового дисплея
// назначение контактов на I2C LCD:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // bl - backlight, blpol - полярность подсветки

void setup()   
{
  Serial.begin(9600);  // 

  lcd.begin(20,4);   // инициализация lcd 20 символьного 4 строкового дисплея, подсветка включена
  delay(2000);
  lcd.noBacklight(); // выключить подсветку
  delay(2000);
  lcd.backlight();   // включить подсветку
  
  // нумерация позиции курсора для строки и символа начинается с 0  
  lcd.setCursor(0,0); // начало с символа 1 строка 1
  lcd.print("Char 1, Row 1");
  delay(1000);
  lcd.setCursor(1,1); // начало с символа 2 строка 2
  lcd.print("Char 2, Row 2");
  delay(1000);  
  lcd.setCursor(2,2); // начало с символа 3 строка 3
  lcd.print("Char 3, Row 3");
  lcd.setCursor(3,3); // начало с символа 4 строка 4
  delay(1000);   
  lcd.print("Char 4, Row 4");
  delay(5000);
  lcd.clear(); // очистка дисплея
  lcd.setCursor(0,0); 
  lcd.print("www.umnyjdomik.ru");
  lcd.setCursor(0,1); 
  lcd.print("Start Serial Monitor");
  lcd.setCursor(0,2);
  lcd.print("Type chars on keyboard");    
}

void loop()  
{
  {
      if (Serial.available()) {      
      delay(100); // задержка для получения всего сообщения
      lcd.clear();
      // чтение всех возможных символов
      while (Serial.available() > 0) {
        lcd.write(Serial.read()); // отображение каждого символа на дисплее LCD
      }
    }
  }
}
