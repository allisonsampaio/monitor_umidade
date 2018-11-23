
#include <LiquidCrystal.h>
#define pino_sinal_analogico A0
#define pino_led_vermelho 8
#define pino_led_amarelo 9
#define pino_led_verde 10

LiquidCrystal lcd(2,3,4,5,6,7);

int valor_analogico;
 
void setup()
{
  Serial.begin(9600);
  pinMode(pino_sinal_analogico, INPUT);
  pinMode(pino_led_vermelho, OUTPUT);
  pinMode(pino_led_amarelo, OUTPUT);
  pinMode(pino_led_verde, OUTPUT);

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
  lcd.clear();                //Limpa a tela do LCD
  lcd.print("Microcontrolados");  //Escreve Vida de Silício
  lcd.setCursor(6,1);         //Posiciona o cursor na posição (6,1)
  lcd.setCursor(8,1);         //Posiciona o cursor na posição (8,1)
}
 
void loop()
{
  //Le o valor do pino A0 do sensor
  valor_analogico = analogRead(pino_sinal_analogico);
 
  //Mostra o valor da porta analogica no serial monitor
  Serial.print("Porta analogica: ");
  Serial.print(valor_analogico);
 
  //Solo umido, acende o led verde
  if (valor_analogico > 0 && valor_analogico < 670)
  {
    Serial.println(" Status: Solo umido");
    apagaleds();
    digitalWrite(pino_led_verde, HIGH);
  }
 
  //Solo com umidade moderada, acende led amarelo
  if (valor_analogico > 670 && valor_analogico < 800)
  {
    Serial.println(" Status: Umidade moderada");
    apagaleds();
    digitalWrite(pino_led_amarelo, HIGH);
  }
 
  //Solo seco, acende led vermelho
  if (valor_analogico > 800 && valor_analogico < 1024)
  {
    Serial.println(" Status: Solo seco");
    apagaleds();
    digitalWrite(pino_led_vermelho, HIGH);
  }
  delay(100);
}
 
void apagaleds()
{
  digitalWrite(pino_led_vermelho, LOW);
  digitalWrite(pino_led_amarelo, LOW);
  digitalWrite(pino_led_verde, LOW);
}
