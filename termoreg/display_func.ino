void showTemp(float temp) {
  byte sr[4];// массив разрядов индикатора

  //массив изображений 7 сегментных символов от 0 до 9 и пустота
  byte simv[12] = {
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
    B01000000, //-
  };

  byte dot = B10000000;
  
  byte dig1 = (temp < 0) ? simv[11] : simv[10];
  byte dig2 = (temp >= 10 || temp <= -10) ? simv[(int)temp / 10] : simv[10];
  byte dig3 = simv[(int)temp % 10];
  byte dig4 = simv[(int)(temp * 10) % 10];



 writeFourDig(dig1, dig2, dig3 ^ dot, dig4);


}

void writeFourDig(byte dig1, byte dig2, byte dig3, byte dig4) {
  Wire.beginTransmission(0x34); //Пишем в 1 разряд
  Wire.write(dig1);    //
  Wire.endTransmission();       // закрываем сессию

  Wire.beginTransmission(0x35); //Пишем вo 2 разряд
  Wire.write(dig2);
  Wire.endTransmission(); // закрываем сессию

  Wire.beginTransmission(0x36); //Пишем в 3 разряд
  Wire.write(dig3);
  Wire.endTransmission(); // закрываем сессию

  Wire.beginTransmission(0x37); //Пишем в 4 разряд
  Wire.write(dig4);
  Wire.endTransmission(); // закрываем сессию
}
