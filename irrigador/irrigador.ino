#include <LiquidCrystal.h>
#include <DS1307.h>
#define pino_sinal_analogico A0
#define pino_led_vermelho 8
#define pino_led_amarelo 9
#define pino_led_azul 10
#define buzzer 11

DS1307 rtc(A4, A5);

LiquidCrystal lcd(2,3,4,5,6,7);
int valor_analogico;
int last_status;
int status_buzzer = 0;
 
void setup()
{

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

  //As linhas abaixo setam a data e hora do modulo
  //e podem ser comentada apos a primeira utilizacao
  rtc.setTime(20, 37, 0);     //Define o horario
  rtc.setDate(5, 12, 2018);   //Define o dia, mes e ano
   
  //Definicoes do pino SQW/Out
  rtc.setSQWRate(SQW_RATE_1);
  rtc.enableSQW(true);
}
 
void loop()
{
  
  if(status_buzzer == 1){
  	digitalWrite(buzzer,HIGH);
  }else{
  	digitalWrite(buzzer,LOW);
  }

  //Le o valor do pino A0 do sensor
  valor_analogico = analogRead(pino_sinal_analogico);
 
  //Solo umido, acende o led azul
  if (valor_analogico > 0 && valor_analogico < 670 && last_status != 3)
  {
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("SOLO UMIDO");
    lcd.setCursor(0, 1);
    lcd.print(rtc.getDateStr());
    lcd.print(' ');
    lcd.print(rtc.getTimeStr());
    apagaleds();
    digitalWrite(pino_led_azul, HIGH);
    last_status = 3;
    status_buzzer = 0;
  }
 
  //Solo com umidade moderada, acende led amarelo
  if (valor_analogico > 670 && valor_analogico < 900 && last_status != 2)
  {
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("SOLO MODERADO");
    lcd.setCursor(0, 1);
    lcd.print(rtc.getDateStr());
    lcd.print(' ');
    lcd.print(rtc.getTimeStr());
    apagaleds();
    digitalWrite(pino_led_amarelo, HIGH);
    last_status = 2;
    status_buzzer = 0;
  }
 
  //Solo seco, acende led vermelho
  if (valor_analogico > 900 && valor_analogico < 1024 && last_status != 1)
  {
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("SOLO SECO");
    lcd.setCursor(0, 1);
    lcd.print(rtc.getDateStr());
    lcd.print(' ');
    lcd.print(rtc.getTimeStr());
    apagaleds();
    digitalWrite(pino_led_vermelho, HIGH);
    last_status = 1;
    status_buzzer = 1;
  }
  if(digitalRead(12) == HIGH){
    	status_buzzer = 0;
  }


}
 
void apagaleds()
{
  digitalWrite(pino_led_vermelho, LOW);
  digitalWrite(pino_led_amarelo, LOW);
  digitalWrite(pino_led_azul, LOW);
}