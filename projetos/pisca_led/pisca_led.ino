// Primeiro projeto: pisca o LED "L" da placa (ligado ao pino 13).
// Não precisa montar nada na protoboard.
// Também manda mensagens pelo USB, que dá para ler no monitor serial.

const int PINO_LED = 13;   // LED "L" da placa
const int INTERVALO = 500; // milissegundos aceso e apagado

int contador = 0;

void setup() {
  pinMode(PINO_LED, OUTPUT);  // o pino 13 vai controlar o LED
  Serial.begin(9600);         // velocidade da conversa com o Mac
  Serial.println("Ola, bancada! O Uno ligou.");
}

void loop() {
  digitalWrite(PINO_LED, HIGH);  // 5 V no pino: acende
  delay(INTERVALO);
  digitalWrite(PINO_LED, LOW);   // 0 V no pino: apaga
  delay(INTERVALO);

  contador++;
  Serial.print("Piscadas: ");
  Serial.println(contador);
}
