# lcd_show

Um pequeno show que roda sem parar no display, com 5 LEDs acompanhando:

1. **Abertura:** "Bancada ON" e "Arduino Uno R3" aparecem letra por letra; os LEDs enchem como uma barra.
2. **Corrida (20 s):** um bonequinho corre na linha de baixo e pula sozinho os cactos que vêm da direita. O placar fica no canto. Uma luz vai e volta nos LEDs, e todos piscam a cada pulo.
3. **Coração:** três corações batem na tela e o LED vermelho bate junto.
4. **Letreiro:** "Proximo passo: ESP32 com Wi-Fi e Bluetooth!" passa na linha de baixo; os LEDs mostram o progresso.

O bonequinho, os cactos e os corações são desenhados pixel a pixel e gravados no display.

## Material

- Tudo do [lcd_ola](../lcd_ola/) (display, 11 fios, resistor de 1 kΩ do contraste)
- 5 LEDs: 1 vermelho, 2 amarelos, 2 verdes
- 5 resistores para os LEDs: **470 Ω** (amarelo, violeta, marrom, dourado) é o ideal; na montagem real foram usados de **1 kΩ**, que também funcionam
- 5 fios a mais

Sem os LEDs montados, o display funciona do mesmo jeito.

## Ligação

Desenho completo em [`montagem.html`](montagem.html).

**Parte 1, display:** exatamente como no [lcd_ola](../lcd_ola/README.md#ligação).

**Parte 2, LEDs:** cada LED usa duas fileiras vizinhas da protoboard. A perna longa (+) vai na coluna **d** da fileira de número menor; a curta (−), na coluna **d** da seguinte. O resistor vai do furo **a** da fileira da perna curta até o trilho azul (−).

| LED | Fileiras | Fio do Arduino |
|---|---|---|
| vermelho | 2 e 3 | pino **~10** → a2 |
| amarelo | 5 e 6 | pino **~9** → a5 |
| amarelo | 8 e 9 | pino **8** → a8 |
| verde | 11 e 12 | pino **7** → a11 |
| verde | 14 e 15 | pino **~6** → a14 |

Não precisa de fio de GND para os LEDs: o trilho azul (−) já está ligado ao GND pelo fio do contraste do display.

O resistor do contraste continua na fileira **20** e é **exclusivo do display**. Os LEDs têm cada um o seu.

## Como o código funciona

- **Caracteres desenhados:** cada letra do display é uma grade de 5×8 pixels. `lcd.createChar(numero, desenho)` grava até 8 desenhos próprios, e `lcd.write(numero)` mostra um deles. Os desenhos estão no começo do arquivo, em binário: cada `1` é um pixel aceso.

  ```
  0b01110   .###.
  0b01110   .###.
  0b00100   ..#..    bonequinho
  0b11111   #####
  0b00100   ..#..
  0b01010   .#.#.
  0b10001   #...#
  ```

- **Corrida:** um vetor `temCacto[16]` guarda onde há cacto. A cada passo tudo anda uma casa para a esquerda e às vezes nasce um cacto na direita. Quando um cacto está a uma casa do bonequinho, ele pula por 2 passos.
- **LEDs como barra:** `barra(n)` acende os `n` primeiros LEDs; `soUm(posicao)` acende só um.
- **PWM:** `analogWrite(10, brilho)` controla o brilho do LED vermelho (0 a 255). Só funciona nos pinos com `~` (3, 5, 6, 9, 10, 11): eles ligam e desligam muito rápido para simular brilhos intermediários.
- **`map()`:** converte a posição do letreiro (0 até o fim) em quantos LEDs acender (0 a 5).
- **`randomSeed(analogRead(A0))`:** o pino A0 solto lê ruído elétrico, e isso faz os cactos saírem numa ordem diferente a cada vez.

O programa também manda pelo USB o nome de cada cena e o placar da corrida (monitor serial a 9600).

## Enviar

De dentro de `projetos/`:

```bash
arduino-cli compile --fqbn arduino:avr:uno lcd_show
arduino-cli upload -p /dev/cu.usbmodem212201 --fqbn arduino:avr:uno lcd_show
```

## O que aconteceu na prática

- **LEDs muito fracos:** os primeiros resistores eram de 100 kΩ. Trocados pelos de 1 kΩ, acenderam bem.
- **Display apagou depois de montar os LEDs:** o resistor do contraste tinha sido tirado da fileira 20 para usar num LED. Recolocado, a tela voltou.
- Os pinos 9, 8, 7 e 6 ficam ao lado dos fios do display (5, 4, 3, 2): espetar um fio um furo para o lado troca as funções. Confira o número impresso antes.
