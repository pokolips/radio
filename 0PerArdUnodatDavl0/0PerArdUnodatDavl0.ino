 #include <SFE_BMP180.h>
#include <Wire.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <OneWire.h>  //библиотека работы шины для ds18b20
#include <DallasTemperature.h> //библиотека для считывания температуры с ds18b20
   
const uint64_t pipe = 0xF0F1F2F3F4LL; //идентификатор передачи данных.
RF24 radio(9, 10); //указываем пины подключения передатчика.
OneWire  oneWire(2);  //указываем пины для подключения датчиков ds18b20.
DallasTemperature ds(&oneWire); //активируем библиотеку ds.
SFE_BMP180 pressure;

DeviceAddress sensor1 = {0x28, 0xFF, 0x5E, 0xBB, 0x82, 0x16, 0x3, 0xF3}; //адрес первого датчика.
DeviceAddress sensor2 = {0x28, 0xFF, 0xE, 0x6, 0x85, 0x16, 0x5, 0xCD}; //адрес второго датчика.
DeviceAddress sensor3 = {0x28, 0xFF, 0x46, 0xF4, 0x82, 0x16, 0x3, 0xCB}; //адрес второго датчика.
 
void setup(){
  Serial.begin(9600); //открываем пот обмена данными.
  ds.begin();  //запускаем функцию считывания температуры ds.
  
  radio.begin(); //делаем инициализацию.
  delay(2); //ждем реакции модуля передачи данных.
  radio.setChannel(9); // канал для передачи данных (0-127).
  radio.setDataRate(RF24_1MBPS); //указываем на какой скорости будут общаться модули.
  radio.setPALevel(RF24_PA_HIGH); //указываем мощность передатчика. 
  radio.openWritingPipe(pipe); // открываем трубу на передачу.
}
 
 
void loop(){ 
   char status;
  double T,P,p0,a;

  ds.requestTemperatures(); // считываем температуру с датчиков
  


   status = pressure.startTemperature();       // Считывание показания
  if(status!=0){                              // Если значение status не 0, выполняем следующию команду.
     delay(status);                           // Ждем     
     status = pressure.getTemperature(T);     // Полученые показания, сохраняем в переменную T
      if(status!=0){                          // Если все хорошо, функция вернет 1, иначе вернет 0
         Serial.print("Temperature: ");       // Печать текста "Температура"
         Serial.print(T,2);                   // Печать показания переменной "Т"
         Serial.println(" C, ");              // Печать текста "С"
        //  lcd.println(F("Температура - "));
        //  lcd.print(T,2);                   // Печать показания переменной "Т"
        //  lcd.println(" C, ");
/* Определяем показания атмосферного давления
 * Параметр указывает расширение, от 0 до 3 (чем больше расширение, тем больше точность, тем долше ждать)
 * Если все хорошо, функция pressure.startTemperature вернет status с количеством милисикунд
 * которые нужно подождать. Ксли какае то проблема, то функция вернет 0.
*/
 
  status = pressure.startPressure(3);         // Считывание показания
  if(status!=0){                              // Если значение status не 0, выполняем следующию команду.
     delay(status);                           // Ждем
     status = pressure.getPressure(P,T);      // Полученные показания, сохраняем в переменную P
      if(status!=0){                          // Если все хорошо, функция вернет 1, иначе вернет 0 
         Serial.print("Absolute pressure: "); // Печать текста "Атмосферное давление"
          Serial.print(P,2);                  // Печать показания переменной mBar
          Serial.print(" mbar, ");            // Печать текста "mBar"
          Serial.print(P*0.7500637554192,2);  // Печать показания в mmHg
          // lcd.print(" Давление - ");           // Печать текста "mmHg"
          // lcd.print(P*0.7500637554192,2);  // Печать показания в mmHg
          Serial.println(" - мм.рт/ст");}           // Печать текста "mmHg"
 
  else Serial.println("error retrieving pressure measurement\n");}    // Ошибка получения давления
  else Serial.println("error starting pressure measurement\n");}      // Ошибка запуска получения давления
  else Serial.println("error retrieving temperature measurement\n");} // Ошибка получения температуры
  else Serial.println("error starting temperature measurement\n");    // Ошибка запуска получения температуры
  delay(5000);                                                        // Пауза в 5с

  float data [3];  //создаем массив из 3-х переменных.
  data[0] = float(ds.getTempC(sensor1)); //присваиваем первому значению массива данные с sensor1 (за один раз можно отправить до 32 байт).
  data[1] = float(ds.getTempC(sensor2)); //присваиваем второму значению массива данные с sensor2 (за один раз можно отправить до 32 байт).
  data[2] = float((T,2)); //присваиваем второму значению массива данные с sensor3 (за один раз можно отправить до 32 байт).
  data[3] = float((P*0.7500637554192,2)); //присваиваем второму значению массива данные с sensor3 (за один раз можно отправить до 32 байт).
  radio.write(&data, sizeof(data)); // передаем массив и его размер.
   Serial.println(data[0]); //выводим данные в сериал порт для проверки.
   Serial.println(data[1]); //выводим данные в сериал порт для проверки.
   Serial.println(data[2]); //выводим данные в сериал порт для проверки.
   Serial.println(data[3]); //выводим данные в сериал порт для проверки.
  //  Serial.println(data[2]); //выводим данные в сериал порт для проверки.
   Serial.println(); //пробел.
   delay(500); //ждем пол секунды. 
}

