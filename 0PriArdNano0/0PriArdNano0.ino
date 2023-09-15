#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C dislpl(0x27,16,2);
   
  const uint64_t pipe = 0xF0F1F2F3F4LL; //первый идентификатор передачи данных.
  RF24 radio(9, 10); //указываем пины подключения передатчика.
 
void setup(){
  Serial.begin(9600); //открываем пот обмена данными.
  radio.begin(); //делаем инициализацию.
  delay(2); //ждем реакции модуля передачи данных.
  radio.setChannel(9); // канал для передачи данных (0-127).
  radio.setDataRate(RF24_1MBPS); //указываем на какой скорости будут общаться модули.
  radio.setPALevel(RF24_PA_HIGH); //указываем мощность передатчика. 
 
  radio.openReadingPipe(1,pipe); //Можно открыть все трубы разом.
  radio.startListening(); //включаем приемник и начинаем слушать трубу.
   dislpl.init();
    // заголовки на дисплей
    dislpl.backlight();
}
 
 
void loop(){ 
  float data [3];  //создаем массив из 3-х переменных.
  if (radio.available()){ //проверяем не пришло ли чего в буфер.
  radio.read(&data, sizeof(data)); //читаем массив и указываем сколько пришло.
   dislpl.setCursor(0,0);
        dislpl.print(data[0]);
        dislpl.setCursor(0,1);
        dislpl.print(data[1]);
        dislpl.setCursor(6,1); // нужно 7,1
        dislpl.print(data[2]);
  /* Serial.println(data[0]); //выводим данные в сериал порт для проверки.
   Serial.println(data[1]); //выводим данные в сериал порт для проверки.
   Serial.println(data[2]); //выводим данные в сериал порт для проверки.
   Serial.println(); //пробел. */
  }
}

