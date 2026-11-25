// Show no display LCD 16x2 + 5 LEDs: abertura digitada, corridinha automática,
// coração batendo e letreiro passando. Repete para sempre.
// Montagem: veja montagem.html nesta pasta (LCD do lcd_ola + 5 LEDs na protoboard).
// Sem os LEDs montados, o display funciona do mesmo jeito.

#include <LiquidCrystal.h>

// RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// LEDs em forma de barra, do primeiro ao último: verde, verde, amarelo, amarelo, vermelho
const int LEDS[5] = {6, 7, 8, 9, 10};
const int LED_VERMELHO = 10;  // pino com PWM: dá para controlar o brilho

// Acende os LEDs de 0 até quantos-1 e apaga o resto (barra)
void barra(int quantos) {
  for (int i = 0; i < 5; i++) digitalWrite(LEDS[i], i < quantos ? HIGH : LOW);
}

// Acende só um LED (posição 0 a 4)
void soUm(int posicao) {
  for (int i = 0; i < 5; i++) digitalWrite(LEDS[i], i == posicao ? HIGH : LOW);
}

// ---- Desenhos próprios (5x8 pixels cada; 1 = pixel aceso) ----
byte corredorA[8] = {0b01110, 0b01110, 0b00100, 0b11111, 0b00100, 0b01010, 0b10001, 0b00000};
byte corredorB[8] = {0b01110, 0b01110, 0b00100, 0b01110, 0b10101, 0b00100, 0b01010, 0b01010};
byte pulando[8]   = {0b01110, 0b01110, 0b10101, 0b01110, 0b00100, 0b01010, 0b10001, 0b00000};
byte cacto[8]     = {0b00100, 0b10101, 0b10101, 0b10111, 0b11100, 0b00100, 0b00100, 0b00100};
byte coracao[8]   = {0b00000, 0b01010, 0b11111, 0b11111, 0b11111, 0b01110, 0b00100, 0b00000};
byte coracaoP[8]  = {0b00000, 0b00000, 0b01010, 0b01110, 0b01110, 0b00100, 0b00000, 0b00000};

// Números das "letras" personalizadas guardadas no display
const byte CORRE_A = 0, CORRE_B = 1, PULO = 2, CACTO = 3, CORACAO = 4, CORACAO_P = 5;

// Escreve um texto letra por letra, como se alguém estivesse digitando
void digitar(const char* texto, int coluna, int linha, int pausa) {
  lcd.setCursor(coluna, linha);
  for (int i = 0; texto[i] != '\0'; i++) {
    lcd.print(texto[i]);
    delay(pausa);
  }
}

// Cena 1: abertura
void cenaAbertura() {
  Serial.println("cena: abertura");
  lcd.clear();
  for (int i = 0; i <= 5; i++) { barra(i); delay(120); }  // LEDs enchem como uma barra
  digitar("Bancada ON", 3, 0, 120);
  digitar("Arduino Uno R3", 1, 1, 80);
  delay(1500);
  barra(0);
}

// Cena 2: o bonequinho corre e pula os cactos sozinho
void cenaCorrida(unsigned long duracao) {
  Serial.println("cena: corrida");
  bool temCacto[16] = {false};
  int pulo = 0;       // quantos passos ainda no ar
  int pontos = 0;
  int semCacto = 0;   // passos desde o último cacto (evita cactos colados)
  byte passo = 0;
  unsigned long inicio = millis();

  lcd.clear();
  while (millis() - inicio < duracao) {
    // tudo anda uma casa para a esquerda
    for (int i = 0; i < 15; i++) temCacto[i] = temCacto[i + 1];
    semCacto++;
    temCacto[15] = (semCacto >= 4 && random(10) < 4);
    if (temCacto[15]) semCacto = 0;

    if (temCacto[1]) pontos++;              // cacto acabou de passar pelo bonequinho
    if (pulo > 0) pulo--;
    if (pulo == 0 && temCacto[3]) pulo = 2; // cacto chegando: pula

    // linha de cima: bonequinho no ar + placar
    lcd.setCursor(0, 0);
    for (int c = 0; c < 10; c++) {
      if (c == 2 && pulo > 0) lcd.write(PULO);
      else lcd.print(' ');
    }
    lcd.print("Pts:");
    lcd.print(pontos);
    lcd.print("  ");

    // linha de baixo: chão com cactos e bonequinho correndo
    lcd.setCursor(0, 1);
    for (int c = 0; c < 16; c++) {
      if (c == 2 && pulo == 0) lcd.write(passo % 2 ? CORRE_A : CORRE_B);
      else if (temCacto[c]) lcd.write(CACTO);
      else lcd.print(' ');
    }

    // LEDs: luz correndo de um lado para o outro; todos piscam quando ele pula
    if (pulo > 0) barra(5);
    else soUm(passo % 8 < 5 ? passo % 8 : 8 - passo % 8);

    passo++;
    delay(170);
  }
  barra(0);
  Serial.print("pontos: ");
  Serial.println(pontos);
}

// Cena 3: coração batendo
void cenaCoracao() {
  Serial.println("cena: coracao");
  lcd.clear();
  lcd.setCursor(1, 1);
  lcd.print("Meu 1o projeto");
  for (int batida = 0; batida < 8; batida++) {
    byte desenho = (batida % 2 == 0) ? CORACAO : CORACAO_P;
    for (int c = 5; c <= 9; c += 2) {
      lcd.setCursor(c, 0);
      lcd.write(desenho);
    }
    analogWrite(LED_VERMELHO, batida % 2 == 0 ? 255 : 20);  // LED vermelho bate junto
    delay(batida % 2 == 0 ? 250 : 450);
  }
  analogWrite(LED_VERMELHO, 0);
}

// Cena 4: letreiro passando na linha de baixo
void cenaLetreiro() {
  Serial.println("cena: letreiro");
  const char* mensagem = "                Proximo passo: ESP32 com Wi-Fi e Bluetooth!                ";
  int tamanho = strlen(mensagem);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Novidades");
  for (int inicio = 0; inicio <= tamanho - 16; inicio++) {
    lcd.setCursor(0, 1);
    for (int i = 0; i < 16; i++) lcd.print(mensagem[inicio + i]);
    barra(map(inicio, 0, tamanho - 16, 0, 5));  // barra de progresso do letreiro
    delay(220);
  }
  barra(0);
}

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 5; i++) pinMode(LEDS[i], OUTPUT);
  lcd.begin(16, 2);
  lcd.createChar(CORRE_A, corredorA);
  lcd.createChar(CORRE_B, corredorB);
  lcd.createChar(PULO, pulando);
  lcd.createChar(CACTO, cacto);
  lcd.createChar(CORACAO, coracao);
  lcd.createChar(CORACAO_P, coracaoP);
  randomSeed(analogRead(A0));  // pino solto dá um número diferente a cada vez
  Serial.println("lcd_show ligado");
}

void loop() {
  cenaAbertura();
  cenaCorrida(20000);  // 20 segundos de corrida
  cenaCoracao();
  cenaLetreiro();
}
