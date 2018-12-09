float getTemp()
{
  const byte sens[8] = { 0x28, 0xFF, 0x70, 0x0F, 0x60, 0x17, 0x04, 0xF9 };
  static unsigned long prevTime = 0;
  if (millis() - prevTime > 60000)
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
        byte bufData[9];
        sensDs.reset();
        sensDs.select(sens);
        sensDs.write(0xBE);
        sensDs.read_bytes(bufData, 9);
        if ( OneWire::crc8(bufData, 8) == bufData[8] ) // проверка CRC
        {  
          // данные правильные
          int Temp = (bufData[1] << 8) + bufData[0];
          return Temp / 16.0;
        }   
    }
  }
}
