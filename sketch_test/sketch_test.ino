#include <OneWire.h>
#include <DallasTemperature.h>
 
OneWire oneWire(2); // вход датчиков 18b20
DallasTemperature ds(&oneWire);

DeviceAddress sensor0;

void setup() {
  Serial.begin(9600);
  ds.begin();
  
    // показываем сколько датчиков нашли на шине
  Serial.print("Found ");
  Serial.print(ds.getDeviceCount(), DEC);
  Serial.println(" devices.");

    // достаем адрес датчика с индесом 0 
  if (!ds.getAddress(sensor0, 0)){ 
    Serial.println("Unable to find address for Device 0"); 
  } 

    // отпаравляем адрес из массива в монитор порта
  Serial.print("address sensor 0: ");   
  for (uint8_t i = 0; i < 8; i++)  {  
    Serial.print("0x");   
    Serial.print(sensor0[i], HEX);
    Serial.print(", ");
  }
  Serial.println();
 
    // устанавливаем разрешение датчика 11 бит (может быть 9, 10, 11, 12) 
    // на точность измерения температуры показатель не влияет.
 ds.setResolution(sensor0, 12); 

 
}

void loop() {
   ds.requestTemperatures();                  // считываем температуру с датчиков
     
    Serial.print("Sensor 0: "); 
    Serial.print(ds.getTempC(sensor0));      // отправляем температуру
    Serial.println("C  ");                    
 
    
 //   delay(1000);
}
