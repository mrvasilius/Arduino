#include <EEPROM.h>

//#include <Dhcp.h>
//#include <Dns.h>
#include <Ethernet.h>
//#include <EthernetClient.h>
//#include <EthernetServer.h>
//#include <EthernetUdp.h>

//#include <Wire.h>
#include <OneWire.h>
#include <LiquidCrystal_I2C.h>


#define ledGood 7
#define ledWarning 6
#define ledDanger 5
#define ONE_WIRE_BUS 2
#define DANGER_TEMP 40
#define WARNING_TEMP 30
#define REQ_BUF_SZ 50
#define REQ_METHOD_BUF_SZ 7



const byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDA, 0x02 };
IPAddress ip(192, 168, 200, 27);
EthernetServer server(80);
float temp[3];
int period = 1000;
char HTTP_req_method[REQ_METHOD_BUF_SZ] = {0};
char HTTP_req[REQ_BUF_SZ] = {0}; // buffered HTTP request stored as null terminated string
byte req_index = 0; // index into HTTP_req buffer
byte HTTP_flag = 0;

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
OneWire  sensDs(ONE_WIRE_BUS);
const byte sens[3][8] = {
  { 0x28, 0xA1, 0x6E, 0xEE, 0x09, 0x00, 0x00, 0x14 },
  { 0x28, 0xF2, 0xFE, 0xEC, 0x09, 0x00, 0x00, 0xD3 },
  { 0x28, 0x20, 0xC9, 0xED, 0x09, 0x00, 0x00, 0x23 }
};

void setup()
{
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.begin(9600);
  lcd.begin(20, 4);
  lcd.backlight();
  pinMode(ledGood, OUTPUT);
  pinMode(ledWarning, OUTPUT);
  pinMode(ledDanger, OUTPUT);
}

void loop()
{
  EthernetClient client = server.available(); //принимаем данные, посылаемые клиентом
  if (client) {
    boolean currentLineIsBlank = true;
    while (client.connected()) 
    {
      if (client.available())
      {
        char c = client.read();
        parseReq(c);
        if (c == '\n' && currentLineIsBlank)
        {
          if (StrContains(HTTP_req_method, "GET"))
          {
            client.println(F("HTTP/1.1 200 OK"));
            client.println(F("Content-Type: application/json"));
            client.println(F("Connnection: close"));
            client.println();
            client.println(F("{\"Status\":\"OK\"}"));             
          }
          else if (StrContains(HTTP_req, "GET /temp "))
          {
            client.println(F("HTTP/1.1 200 OK"));
            client.println(F("Content-Type: application/json"));
            client.println(F("Connnection: close"));
            client.println();
            client.print(F("{"));
            client.print(F("\"Status\":\"OK\""));             
            client.print(F(","));
//            if (StrContains(HTTP_req, "GET /temp? "))
            client.print(F("\"Temperature\":["));
            for (int i  = 0; i < 3; ++i)
            {
              client.print(F("\""));
              client.print(temp[i]);
              client.print(F("\""));
              if (i < 2)
              { 
                client.print(F(","));
              }
            }
            client.print(F("]"));
            client.print(F("}"));
          }
          else if (StrContains(HTTP_req, "GET /test "))
          {
            client.println(F("HTTP/1.1 200 OK"));
            client.println(F("Content-Type: application/json"));
            client.println(F("Connnection: close"));
            client.println();
          }
          HTTP_flag = 0;
          req_index = 0;
          StrClear(HTTP_req, REQ_BUF_SZ);
          StrClear(HTTP_req_method, REQ_METHOD_BUF_SZ);
                    
          break;
        }
      }       
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();    

  }
  getTemp();  
}
void getTemp()
{
  static unsigned long prevTime = 0;
  if (millis() - prevTime > period)
  {
    static boolean flagDall = 0;
    prevTime = millis();
    flagDall =! flagDall;
    if (flagDall)
    {
      // Делаем запрос данных
      sensDs.reset();
      sensDs.write(0xCC);
      sensDs.write(0x44);
    }
    else
    {
      static int count;
      count++;
      lcd.setCursor(0, 3);
      lcd.print(count);
      for (int i  = 0; i < 3; ++i) 
      {
        byte bufData[9];
        sensDs.reset();
        sensDs.select(sens[i]);
        sensDs.write(0xBE);
        sensDs.read_bytes(bufData, 9);
        if ( OneWire::crc8(bufData, 8) == bufData[8] ) // проверка CRC
        {  
          // данные правильные
          int Temp = (bufData[1] << 8) + bufData[0];
          temp[i] = Temp / 16.0;
        } 
        else
        {
          temp[i] = (float)-273.15;
        }    
      }
    }
    lcd.setCursor(0, 0);
    lcd.print(F("Temp1 = "));
    lcd.print(temp[0]);
    lcd.print(F("C  "));
    lcd.setCursor(0, 1);
    lcd.print(F("Temp2 = "));
    lcd.print(temp[1]);
    lcd.print(F("C  "));
    lcd.setCursor(0, 2);
    lcd.print(F("Temp3 = "));
    lcd.print(temp[2]);
    lcd.print(F("C  "));
    checkAlarm(temp);
  }
}
void StrClear(char *str, char length)
{
  for (int i = 0; i < length; i++) {
    str[i] = 0;
  }
}
void parseReq (char *c)
{
  if (HTTP_flag == 1)
  {
    if (c != ' ') 
    {
      HTTP_req[req_index] = c; // save HTTP request character
      req_index++;
    }
    else
    {
      HTTP_flag = 2;
    }
   }
   if (HTTP_flag == 0)
   {
    if (c != ' ')
    {
      HTTP_req_method[req_index] = c;
      req_index++;
    }
    else
    {
      HTTP_flag = 1;
      req_index = 0;
    }
  }
}
char StrContains(char *str, char *sfind)
{
  char found = 0;
  char index = 0;
  char len;
  len = strlen(str);
  if (strlen(sfind) > len) {
    return 0;
  }
  while (index < len) {
    if (str[index] == sfind[found]) {
      found++;
      if (strlen(sfind) == found) {
        return 1;
      }
    }
    else {
      found = 0;
    }
    index++;
  }
  return 0;
}
void checkAlarm(float temp[])
{
  if (temp[0] < WARNING_TEMP && temp[1] < WARNING_TEMP && temp[2] < WARNING_TEMP)
  {
    digitalWrite(ledDanger, LOW);
    digitalWrite(ledWarning, LOW);
    digitalWrite(ledGood, HIGH);
  }
  else if (temp[0] < DANGER_TEMP && temp[1] < DANGER_TEMP && temp[2] < DANGER_TEMP)
  {
    digitalWrite(ledGood, LOW);
    digitalWrite(ledDanger, LOW);
    digitalWrite(ledWarning, HIGH);
  }
  else
  {
    digitalWrite(ledGood, LOW);
    digitalWrite(ledWarning, LOW);
    digitalWrite(ledDanger, HIGH);
  }
}
//int freeRam () {
//  extern int __heap_start, *__brkval;
//  int v;
//  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
//}
