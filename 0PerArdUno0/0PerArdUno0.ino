#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <OneWire.h>  //библиотека работы шины для ds18b20
#include <DallasTemperature.h> //библиотека для считывания температуры с ds18b20
   
const uint64_t pipe = 0xF0F1F2F3F4LL; //идентификатор передачи данных.
RF24 radio(9, 10); //указываем пины подключения передатчика.
OneWire  oneWire(2);  //указываем пины для подключения датчиков ds18b20.
DallasTemperature ds(&oneWire); //активируем библиотеку ds.
 
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
  ds.requestTemperatures(); // считываем температуру с датчиков
  float data [2];  //создаем массив из 2-х переменных.
  data[0] = float(ds.getTempC(sensor1)); //присваиваем первому значению массива данные с sensor1 (за один раз можно отправить до 32 байт).
  data[1] = float(ds.getTempC(sensor2)); //присваиваем второму значению массива данные с sensor2 (за один раз можно отправить до 32 байт).
  data[2] = float(ds.getTempC(sensor3)); //присваиваем второму значению массива данные с sensor2 (за один раз можно отправить до 32 байт).
  radio.write(&data, sizeof(data)); // передаем массив и его размер.
   Serial.println(data[0]); //выводим данные в сериал порт для проверки.
   Serial.println(data[1]); //выводим данные в сериал порт для проверки.
   Serial.println(data[2]); //выводим данные в сериал порт для проверки.
   Serial.println(); //пробел.
   delay(500); //ждем пол секунды. 
}

