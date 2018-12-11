void initDisplay()
{
    Wire.beginTransmission(0x24); //обращаемся к любому порту 0х24-0х27
    Wire.write(0b01000001);       //включаем:яркость 4,8 битный режим, отображение ON
    Wire.endTransmission();       // закрываем сессию
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

void showTemp(float temp)
{
    byte sr[4]; // массив разрядов индикатора

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

void writeFourDig(byte dig1, byte dig2, byte dig3, byte dig4)
{
    Wire.beginTransmission(0x34); //Пишем в 1 разряд
    Wire.write(dig1);             //
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
