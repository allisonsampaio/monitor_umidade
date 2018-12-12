#include <LiquidCrystal.h>
#include <DS1307.h>
#include <Wire.h>
#include "RTClib.h"
#include <EEPROM.h>
#define pino_sinal_analogico A0
#define pino_led_vermelho 8
#define pino_led_amarelo 9
#define pino_led_azul 10
#define buzzer 11

RTC_DS1307 rtc;

LiquidCrystal lcd(2,3,4,5,6,7);
int valor_analogico;
int last_status;
int status_buzzer = 0;
int incomingByte = 0;
int recebido = 0;

void setup()
{

  Wire.begin();
  rtc.begin();

  pinMode(12, INPUT); //linha - teclado
  digitalWrite(12, LOW);
  

  Serial.begin(9600);
  pinMode(pino_sinal_analogico, INPUT);
  pinMode(pino_led_vermelho, OUTPUT);
  pinMode(pino_led_amarelo, OUTPUT);
  pinMode(pino_led_azul, OUTPUT);
  pinMode(buzzer,OUTPUT);

  lcd.begin(16,2);            //Inicializa display de 2 linhas x 16 colunas
  lcd.home();                 //Posiciona cursor no canto superior esquerdo
  lcd.print("Inicializando");            //Executa uma pequena contagem regressiva
  delay(500);
  lcd.print(".");
  delay(500);
  lcd.print(".");
  delay(500);
  lcd.print(".");
  delay(500);
  lcd.clear();

  if(!rtc.isrunning()) {
    Serial.println("RTC parado, vou ajustar com a hora da compilacao...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

    EEPROM.write(0, 670);
    EEPROM.write(4, 900);

    if(EEPROM.read(20) != 12){
      // Configurações iniciais EEPROM para hora
      EEPROM.write(12, 1);
    }
  printMenu();
}
 
void loop()
{
  // Obtem a data e hora correntes e armazena em tstamp
    DateTime tstamp = rtc.now();
  
  int valor1;
  int valor2;

  if(EEPROM.read(14) == 1){
    valor1 = 660;
    valor2 = 800;
  }else if(EEPROM.read(14) == 2){
    valor1 = 680;
    valor2 = 820;
  }else if(EEPROM.read(14) == 3){
    valor1 = 700;
    valor2 = 840;
  }else{
    valor1 = 720;
    valor2 = 860;
  }

  //Serial.println(valor_analogico);

  if(status_buzzer == 1){
    if(EEPROM.read(12) == 1){
      if(status_buzzer == 1 && (tstamp.hour() > 6 && tstamp.hour() < 13)){
        digitalWrite(buzzer,HIGH);
      }
    }else if(EEPROM.read(12) == 2){
      if(status_buzzer == 1 && (tstamp.hour() > 13 && tstamp.hour() < 18)){
        digitalWrite(buzzer,HIGH);
      }
    }else{
      digitalWrite(buzzer,LOW);
    }
  }else{
    digitalWrite(buzzer,LOW);
  }

  //Le o valor do pino A0 do sensor
  valor_analogico = analogRead(pino_sinal_analogico);
 
  //Solo umido, acende o led azul
  if (valor_analogico > 0 && valor_analogico < valor1 && last_status != 3)
  {
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("SOLO UMIDO");

    apagaleds();
    digitalWrite(pino_led_azul, HIGH);
    last_status = 3;
    status_buzzer = 0;
  }
 
  //Solo com umidade moderada, acende led amarelo
  if (valor_analogico > valor1 && valor_analogico < valor2 && last_status != 2)
  {
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("SOLO MODERADO");

    apagaleds();
    digitalWrite(pino_led_amarelo, HIGH);
    last_status = 2;
    status_buzzer = 0;
  }
 
  //Solo seco, acende led vermelho
  if (valor_analogico > valor2 && valor_analogico < 1024 && last_status != 1)
  {
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("SOLO SECO");

    apagaleds();
    digitalWrite(pino_led_vermelho, HIGH);
    last_status = 1;
    status_buzzer = 1;
  }

  //Controle do Buzzer
  if(digitalRead(12) == HIGH){
      status_buzzer = 0;
  }

  tstamp = rtc.now();
  
  //printar no lcd a hora e data
  lcd.setCursor(0, 3);
  lcd.print(tstamp.day());
  lcd.print('/');
  lcd.print(tstamp.month());
  lcd.print(' ');
  lcd.print(tstamp.hour());
  lcd.print(':');
  lcd.print(tstamp.minute());

  menu();
}

void apagaleds(){
  digitalWrite(pino_led_vermelho, LOW);
  digitalWrite(pino_led_amarelo, LOW);
  digitalWrite(pino_led_azul, LOW);
}

void printMenu(){
  Serial.println(" *** MENU *** ");
  Serial.println("(1) - Alterar niveis de umidade");
  Serial.println("(2) - Alterar horario de funcionamento do buzzer");
  Serial.println("Digite a opcao desejada");
}

void await(){
    while(!Serial.available() > 0) {
  }
}

char menu(){
  char caractere;
  char hour;
  char mood;

  caractere = Serial.read();
  
  if(caractere == '1'){
    Serial.println('1');
    Serial.println("  (1) - Extremamente Seco");
    Serial.println("  (2) - Moderadamente Seco");
    Serial.println("  (3) - Quase Umido");
    await();
    mood = Serial.read();
    EEPROM.write(14, mood - '0');

    EEPROM.write(24, 14);
  }

  if(caractere == '2'){
    Serial.println('2');
    Serial.println("  (1) - Ativo das 06h as 13h");
    Serial.println("  (2) - Ativo das 13h as 18h");
    Serial.println("  (3) - Desativado");
    await();
    hour = Serial.read();
    EEPROM.write(12, hour - '0');

    EEPROM.write(20, 12);
  }

  return caractere;
}
