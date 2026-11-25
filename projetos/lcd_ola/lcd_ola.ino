// Escreve no display LCD 16x2 (HA-1602) e conta os segundos.
// Montagem: veja montagem.html nesta pasta (10 fios, sem protoboard).

#include <LiquidCrystal.h>

// Ordem: RS, E, D4, D5, D6, D7  ->  pinos do Arduino ligados a esses pinos do LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  lcd.begin(16, 2);          // 16 colunas, 2 linhas
  lcd.print("Ola, bancada!");  // linha de cima
}

void loop() {
  lcd.setCursor(0, 1);       // coluna 0, linha de baixo
  lcd.print("Ligado: ");
  lcd.print(millis() / 1000);  // segundos desde que ligou
  lcd.print(" s");
  delay(200);
}
