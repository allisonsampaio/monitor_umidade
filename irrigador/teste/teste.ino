#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;

void setup () {
  Wire.begin();
  rtc.begin();

  Serial.begin(9600);
  pinMode(13,OUTPUT);

  if (!rtc.isrunning()) {
    Serial.println("RTC parado, vou ajustar com a hora da compilacao...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void printnn(int n) {
  // imprime um numero com 2 digitos
  // acrescenta zero `a esquerda se necessario
  String digitos= String(n);
  if (digitos.length()==1) {
    digitos="0" + digitos;
  }  
  Serial.print(digitos); 
}  

void loop () {
    // Obtem a data e hora correntes e armazena em tstamp
    DateTime tstamp = rtc.now();

    // Mostra no monitor serial a data e hora correntes
    printnn(tstamp.day());
    Serial.print('/');
    printnn(tstamp.month());
    Serial.print('/');
    Serial.print(tstamp.year(), DEC);
    Serial.print(' ');
    printnn(tstamp.hour());
    Serial.print(':');
    printnn(tstamp.minute());
    Serial.print(':');
    printnn(tstamp.second());
    Serial.println();
    
    // espera alguns segundos piscando o led interno
    for (short i=0; i<=4; i++) {
      digitalWrite(13,HIGH);
      delay(1000);
      digitalWrite(13,LOW);
      delay(1000);
    }  
}