# lcd_ola

Escreve "Ola, bancada!" na linha de cima do display e conta há quantos segundos a placa está ligada na linha de baixo.

É o "olá, mundo" do display: confirma que os 11 fios estão certos antes de fazer qualquer coisa mais elaborada.

## Material

- Arduino Uno + cabo USB
- Display LCD 16x2 **HA-1602** (14 pinos, sem luz de fundo)
- 11 fios macho-macho
- 1 resistor de **1 kΩ** (marrom, preto, vermelho, dourado), para o contraste
- Protoboard (só para o resistor do contraste)

## Ligação

Desenho completo em [`montagem.html`](montagem.html). Vire o display: no verso, ao lado do conector, estão impressos o **1** e o **14**.

| Pino do LCD | Nome | Função | Vai para |
|---|---|---|---|
| 1 | VSS | terra | GND (fileira POWER, borda de baixo do Uno) |
| 2 | VDD | alimentação | 5V (fileira POWER) |
| 3 | V0 | contraste | protoboard **a20** → resistor 1 kΩ do **b20** ao trilho (−) → fio do trilho ao outro GND (POWER) |
| 4 | RS | comando ou texto | pino **12** |
| 5 | RW | ler ou escrever | GND (fileira DIGITAL, ao lado do 13) |
| 6 | E | "confirma" cada dado | pino **11** |
| 7–10 | D0–D3 | dados (modo 8 bits) | **não ligar** |
| 11 | D4 | dado | pino **5** |
| 12 | D5 | dado | pino **4** |
| 13 | D6 | dado | pino **3** |
| 14 | D7 | dado | pino **2** |

O Uno tem exatamente 3 GND, e os 3 são usados aqui.

### Por que só 4 fios de dados

O display aceita dados em 8 bits (D0–D7) ou em 4 bits (só D4–D7). A biblioteca LiquidCrystal usa o modo de 4 bits: manda cada letra em duas metades e economiza 4 pinos do Arduino.

### Por que o resistor no pino 3

O pino 3 controla o contraste. Ligado direto no GND, o contraste fica no máximo e a tela tende a mostrar só retângulos escuros. O display tem resistores internos (R1 a R5, no verso) ligados entre o 5V e o pino 3; o resistor de 1 kΩ até o GND forma um divisor de tensão com eles e baixa um pouco o contraste.

Sem nenhum resistor (pino 3 solto), o contraste vai a zero e a tela fica **totalmente em branco**.

## Como o código funciona

```cpp
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  // RS, E, D4, D5, D6, D7
```

A ordem dos números é a ordem em que a biblioteca espera os pinos. Se um fio estiver em outro pino, basta mudar o número aqui.

- `lcd.begin(16, 2)` avisa o tamanho do display (16 colunas, 2 linhas) e o inicializa.
- `lcd.setCursor(coluna, linha)` posiciona onde a próxima letra vai sair (começa em 0).
- `lcd.print(...)` escreve texto ou número.
- `millis()` devolve quantos milissegundos se passaram desde que a placa ligou.

O display não tem acentos: por isso "Ola" sem acento.

## Enviar

De dentro de `projetos/`:

```bash
arduino-cli compile --fqbn arduino:avr:uno lcd_ola
arduino-cli upload -p /dev/cu.usbmodem212201 --fqbn arduino:avr:uno lcd_ola
```

## O que aconteceu na prática

| Sintoma | Causa | Solução |
|---|---|---|
| Uma fileira de números **3** | display recebendo os comandos de inicialização como texto | conferir os fios de controle: LCD 4 → 12, LCD 5 → GND, LCD 6 → 11 |
| Tela totalmente em branco | o resistor do contraste (fileira 20) foi tirado para usar num LED | recolocar o resistor de 1 kΩ |
| Retângulos escuros | contraste alto demais | resistor maior no pino 3 (dois de 1 kΩ em fila) |
| Texto quase invisível | contraste baixo demais | resistor menor no pino 3 (470 Ω) |

Lembre também que este display não tem luz própria: o texto só aparece com luz ambiente batendo na tela.
