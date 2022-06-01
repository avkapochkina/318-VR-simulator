// библиотеки
#include <ACE128.h> 
#include <ACE128map12345678.h> 
#include <Wire.h> 

// создание объекта для работы с энкодером ace128
ACE128 myACE(2,3,4,5,6,7,8,9, (uint8_t*)encoderMap_12345678);

uint8_t pinPos = 0; 

// константы для энкодера ace128
byte upos = 0;
//byte readACE = 0;
uint8_t oldPos = 255;
// константы для вывода в Serial
int border = 40;

// Представление угла поворота штурвала в байтах
union {
  float f;
  byte b[4];
  } positionS;

// Представление угла поворота ручки в байтах
union {
  float f;
  byte b[4];
  } positionG;
  
void setup() {
      // подключаем сериал
      Serial.begin(9600);

      // настраиваем энкодер ace128
      myACE.begin();    
      pinPos = myACE.acePins();          
      oldPos = pinPos;                
}

int penPosition = 0;
int oldPosition = myACE.upos();

void loop() { 
      // получение значений с энкодера ace128               
      upos = myACE.upos(); 
      if ((oldPosition - upos)==-127){
        penPosition = penPosition - 1;
        }else if ((oldPosition - upos)==127){
        penPosition = penPosition + 1;
        }else if ((oldPosition - upos)==1){
        penPosition = penPosition - 1;
        }else if ((oldPosition - upos)==-1){
        penPosition = penPosition + 1;
      }
      oldPosition = upos;
    
      positionG.f = penPosition;
      //Serial.write(upos);
      
      delay(10);
      //
      Serial.print(String(upos));
}//l
