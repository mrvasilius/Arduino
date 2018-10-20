/*пин №13 связан со встроенным светодиодом на платах Uno,
 * Mega, Nano, Leonardo, Mini и др.
 */
#define LED_PIN 13

void setup() {
  //открытие Serial-порта со скоростью 9600 бод/c
  Serial.begin(9600);

  //настройка пина со светодиодом в режим выхода
  pinMode(LED_PIN, OUTPUT);
}

void loop() {

  //если в буфере Serial-порта пришли байты (символы) и ожидают считывания
  if (Serial.available() != 0) {  
    
    //то считываем один полученный байт (символ)
    byte b = Serial.read();
    
    //если получен символ '1', то светодиод включается
    if (b == 49) digitalWrite(LED_PIN, HIGH);
    
    //если получен символ '0', то светодиод выключается
    if (b == 48) digitalWrite(LED_PIN, LOW);
  }
}
