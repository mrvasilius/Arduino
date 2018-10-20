void checkAlarm()
{
//  if (temp[0] < maxTemp && temp[1] < maxTemp && temp[0] > minTemp && temp[1] > minTemp)
//  {
//    digitalWrite(ledDanger, LOW);
//    digitalWrite(ledWarning, LOW);
//    digitalWrite(ledGood, HIGH);
//  }
////  else if (temp[0] < DANGER_TEMP && temp[1] < DANGER_TEMP && temp[2] < DANGER_TEMP)
////  {
////    digitalWrite(ledGood, LOW);
////    digitalWrite(ledDanger, LOW);
////    digitalWrite(ledWarning, HIGH);
////  }
//  else
//  {
//    digitalWrite(ledGood, LOW);
//    digitalWrite(ledWarning, LOW);
//    digitalWrite(ledDanger, HIGH);
//  }
}
int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
void getTemp()
{
  static unsigned long prevTime = 0;
  if (millis() - prevTime > ((int)period * 1000))
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
      lcd.print("       ");
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
    int cLenght = 26 + countSymbols(temp[0]) + countSymbols(temp[1]) + countSymbols(temp[2]);
//    Serial.println(cLenght);
//    Serial.println(countSymbols(temp[1]));
//    Serial.println(countSymbols(temp[1]));
//    Serial.println(temp[2]);
    EthernetClient client;
    if (client.connect(iotServer, 8086)) {
          client.println("POST /write?db=house HTTP/1.1");
          client.println("Host: 192.168.1.11");
          client.println("User-Agent: Arduino/1.0");
          client.println("Content-Type: text/plain");
          client.println("Connection: close");
          client.print("Content-Length: ");
          client.println(cLenght);
          client.println();
          client.print("room"); //4
          client.print(ROOM); //1
          client.print(" temp1="); //7
          client.print(temp[0],1);
          client.print(",temp2="); //7
          client.print(temp[1],1);
          client.print(",temp3="); //7
          client.println(temp[2],1);
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
//    checkAlarm(temp);
  }
}
int countSymbols(float num)
{
  int count = 0;
  char tmp[8] = {0};
  dtostrf(num, 7, 2, tmp);
  for (int i = 0; i < 8; i++) {
    if (tmp[i] != char(32)) count++;
  }
//  Serial.println(count);
  return count - 2;
}

