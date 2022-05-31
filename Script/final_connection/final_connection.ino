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

// пины энкодера ems22a
const int PIN_CS = 53;
const int PIN_CLOCK = 52;
const int PIN_DATA = 50;
const int PIN_FOR_EMS22a = 10;

// константы для вывода в Serial
int border = 40;

// Входы для драйвера
byte ENA = 48;
byte INA = 31;
byte INB = 30;

// Запрос от компьютера, содержащий информацию о движении яхты 
char inChar;

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

/* 
Константы для создания усилия: отклонения штурвала deviation (либо 0 - нет отклонения, 1 - малое отклонение, 2 - большое отклонения), движения motion (-1 - вперед, 1 - назад, 0 - прямо), поворота руля direct (1 - влево, -1 - вправо, 0 - нет отклонения)
*/

int motion = -1;

/*
Значения для сопротивлений/усилий мотора. small - малое, norm - большое, Resistance - сопротивление, F - сила
*/
int smallResistance = 20;
int smallF = 20;
int normResistance = 150;
int normF = 150;

float leftBorderRuleSmall = 0.4;
float leftBorderRuleBig = 0.1;
float rightBorderRuleSmall = 0.6;
float rightBorderRuleBig = 0.9;
float startSmallRight = 0.5;
float startSmallLeft = 0.5;

void setup() {
      // подключаем сериал
      Serial.begin(9600);

      // настраиваем энкодер ace128
      myACE.begin();    
      pinPos = myACE.acePins();          
      oldPos = pinPos;                 
      
      // настраиваем энкодер ems22a
      pinMode(PIN_CS, OUTPUT);
      pinMode(PIN_CLOCK, OUTPUT);
      pinMode(PIN_DATA, INPUT);
      pinMode(PIN_FOR_EMS22a, OUTPUT);
      digitalWrite(PIN_FOR_EMS22a, HIGH);
      digitalWrite(PIN_CLOCK, HIGH);
      digitalWrite(PIN_CS, LOW);

      // настройка мотора
      pinMode(ENA, OUTPUT);
      pinMode(INA, OUTPUT);
      pinMode(INB, OUTPUT);
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
      digitalWrite(PIN_CLOCK, HIGH);
      

      
      
      positionG.f = penPosition;
      positionS.f = -(pos_s-212)/10; 

      inChar=Serial.read();
      if ((inChar == '3') || (inChar == '2') || (inChar == '1')) {
      
      Serial.write(positionG.b, 4);

  Serial.write(positionS.b, 4);
      // Информация для отладки
      //Serial.println(pos_s);
      }
      pos_s=digitalRead(52);
 Serial.println(pos_s);

      // движение
      // При малом отклонении руля
      if ((((pos_s+1)/1021.0 > startSmallRight) && ((pos_s+1)/1021.0 < rightBorderRuleSmall)) || ((pos_s+1)/1021.0 < startSmallLeft) && ((pos_s+1)/1021.0 > leftBorderRuleSmall)) {
      
            // При движении вперед
            if (inChar == 3){
           
                  // Если руль движется влево
                  if ((pos_s+1)/1021.0 < 0.5){
                   
                        // Вращаем мотор по часовой
                        analogWrite(ENA, smallResistance);
                        digitalWrite(INA, HIGH);
                        digitalWrite(INB, LOW);
                  }else{
                   
                        // Вращаем мотор против часовой
                        analogWrite( ENA, smallResistance );
                        digitalWrite( INA, LOW );
                        digitalWrite( INB, HIGH );
                  }
            }
            // При движении назад
            if (inChar == 1){
                  // Если руль движется влево
                  if ((pos_s+1)/1021.0 < 0.5){
                        // Вращаем мотор против часовой
                        analogWrite(ENA, smallF);
                        digitalWrite(INA, LOW);
                        digitalWrite(INB, HIGH);
                  }else{
                        // Вращаем мотор по часовой
                        analogWrite(ENA, smallF);
                        digitalWrite(INA, HIGH);
                        digitalWrite(INB, LOW);
                  }
            }
      }else
            // При большом отклонении руля
      if (((pos_s+1)/1021.0 > rightBorderRuleBig) || ((pos_s+1)/1021.0 > leftBorderRuleBig)){
            // При движении вперед
            if (inChar == 3){
                  // Если руль движется влево
                  if ((pos_s+1)/1021.0 < 0.5){
                        // Вращаем мотор по часовой
                        analogWrite(ENA, normResistance);
                        digitalWrite(INA, HIGH);
                        digitalWrite(INB, LOW);
                  }else{
                        // Вращаем мотор против часовой
                        analogWrite(ENA, normResistance);
                        digitalWrite(INA, LOW);
                        digitalWrite(INB, HIGH);
                  }
            }
            // При движении назад
            if (inChar == 1){
                  // Если руль движется вправо
                  if ((pos_s+1)/1021.0 > 0.5){
                        // Вращаем мотор против часовой
                        analogWrite(ENA, normF);
                        digitalWrite(INA, LOW);
                        digitalWrite(INB, HIGH);
                  }else{
                        // Вращаем мотор по часовой
                        analogWrite(ENA, normF);
                        digitalWrite(INA, HIGH);
                        digitalWrite(INB, LOW);
                  }
            }
      } else{
            digitalWrite(INA, HIGH);
            digitalWrite(INB, HIGH);
      }
    
}
