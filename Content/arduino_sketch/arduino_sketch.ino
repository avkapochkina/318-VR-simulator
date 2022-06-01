// библиотеки
#include <ACE128.h> 
#include <ACE128map12345678.h> 
#include <Wire.h> 

// создание объекта для работы с энкодером ace128
ACE128 myACE(2,3,4,5,6,7,8,9, (uint8_t*)encoderMap_12345678);

uint8_t pinPos = 0; 

// константы для энкодера ace128
uint8_t upos = 0;
uint8_t oldPos = 255;
/*
// пины энкодера ems22a
const int PIN_CS = 53;
const int PIN_CLOCK = 52;
const int PIN_DATA = 50;
const int PIN_FOR_EMS22a = 10;
*/
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
      /*
      // настраиваем энкодер ems22a
      pinMode(PIN_CS, OUTPUT);
      pinMode(PIN_CLOCK, OUTPUT);
      pinMode(PIN_DATA, INPUT);
      pinMode(PIN_FOR_EMS22a, OUTPUT);
      digitalWrite(PIN_FOR_EMS22a, HIGH);
      digitalWrite(PIN_CLOCK, HIGH);
      digitalWrite(PIN_CS, LOW);
      */
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
     /*  
      // получение значений с энкодера ems22a
      digitalWrite(PIN_CS, HIGH);
      digitalWrite(PIN_CS, LOW);
      int pos_s = 0;

      for (int i=0; i<10; i++) {
            digitalWrite(PIN_CLOCK, LOW);
            digitalWrite(PIN_CLOCK, HIGH); 
            byte b = digitalRead(PIN_DATA) == HIGH ? 1 : 0;
            pos_s += b * pow(2, 10-(i+1));
      }

      for (int i=0; i<6; i++) {
            digitalWrite(PIN_CLOCK, LOW);
            digitalWrite(PIN_CLOCK, HIGH);
      }
      digitalWrite(PIN_CLOCK, LOW);
      digitalWrite(PIN_CLOCK, HIGH);*/
      positionG.f = penPosition;
      //positionS.f = -(pos_s-212)/10; 
      //inChar=Serial.read();
      //if ((inChar == '3') || (inChar == '2') || (inChar == '1')) {
      for(int i = 0; i++; 3){
        Serial.write(positionG.b, i);
      }
       /*  Serial.write(positionS.b, 4);
        // Информация для отладки
        //Serial.println(pos_s);
      }
      pos_s=digitalRead(52);
      Serial.println(pos_s);*/
}
