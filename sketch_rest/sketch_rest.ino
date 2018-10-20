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
#define REQ_BUF_SZ 3
#define REQ_METHOD_BUF_SZ 3
#define BYTE_PERIOD 0
#define BYTE_MAXTEMP 4
#define BYTE_MINTEMP 2
#define ROOM 1



const byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDA, 0x02 };
IPAddress ip(192, 168, 1, 11);
IPAddress iotServer(192, 168, 1, 10);
EthernetServer server(80);
float temp[3];
byte period;
byte maxTemp;
byte minTemp;


char HTTP_req_method[REQ_METHOD_BUF_SZ] = {0};
char HTTP_param[3][REQ_BUF_SZ] = {{0},{0},{0}};
byte req_index = 0;
int stepReq = 0;
int param = -1;
int error = 0;

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
  if (((EEPROM.read(BYTE_PERIOD) ^ 0xE5) == EEPROM.read(BYTE_PERIOD + 1)))
  {
    EEPROM.get(BYTE_PERIOD, period);
  }
  else
  {
    period = 1;
  }
  if (((EEPROM.read(BYTE_MAXTEMP) ^ 0xE5) == EEPROM.read(BYTE_MAXTEMP + 1)))
  {
    EEPROM.get(BYTE_MAXTEMP, maxTemp);
  }
  else
  {
    maxTemp = 30;
  }
  if (((EEPROM.read(BYTE_MINTEMP) ^ 0xE5) == EEPROM.read(BYTE_MINTEMP + 1)))
  {
    EEPROM.get(BYTE_MINTEMP, minTemp);
  }
  else
  {
    minTemp = 16;
  }
//  Serial.println(EEPROM.get(BYTE_PERIOD, period));
//  Serial.println(EEPROM.get(BYTE_MINTEMP, minTemp));
//  Serial.println(EEPROM.get(BYTE_MAXTEMP, maxTemp));
}

void loop()
{
  EthernetClient client = server.available(); //принимаем данные, посылаемые клиентом
  if (client) {
    while (client.connected()) 
    {
      if (client.available())
      {
        char c = client.read();
        if (error == 0)
        {
          error = parseReq(c);
        }
        else if (error <= 2 && error > 0) 
        {
          client.println(F("HTTP/1.1 200 OK"));
          client.println(F("Content-Type: application/json"));
          client.println(F("Connection: close"));
          client.println();
          client.print(F("{\"Status\":\"FAIL\",\"Error\":\""));
          client.print(error);
          client.print(F("\"}"));
          error = 3;
        }
//        Serial.print(c);
//        Serial.println(HTTP_param[1]);
        if (c == '\n')
        {
//          Serial.println(HTTP_param[0]);
          if (error == 0) 
          {
            if (StrContains(HTTP_param[0], "gt"))
            {
              getData(HTTP_param[1], HTTP_param[2], client);
            }
            else if (StrContains(HTTP_param[0], "st"))
            {
              setData(HTTP_param[1], HTTP_param[2], client);
              Serial.println(period);
              Serial.println(minTemp);
              Serial.println(maxTemp);
            }
            else
            {
              client.println(F("HTTP/1.1 200 OK"));
              client.println(F("Content-Type: application/json"));
              client.println(F("Connection: close"));
              client.println();
              client.print(F("{\"Status\":\"FAIL\",\"Error\":\"Unknown command '"));
              client.print(HTTP_param[0]);
              client.print(F("', use 'gt' or 'st'\"}"));
                  
            }
          }
          req_index = 0;
          stepReq = 0;
          param = -1;
//          Serial.println(HTTP_req_method);
//          Serial.println(error);
//          Serial.println(HTTP_param[0]);
//          Serial.println(HTTP_param[1]);
//          Serial.println(HTTP_param[2]);
          StrClear(HTTP_param[0], REQ_BUF_SZ);
          StrClear(HTTP_param[1], REQ_BUF_SZ);
          StrClear(HTTP_param[2], REQ_BUF_SZ);
          StrClear(HTTP_req_method, REQ_METHOD_BUF_SZ);
          error = 0;          
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
  checkAlarm();  
}
