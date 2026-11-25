# Eletrônica

Repositório **educativo**: o registro de alguém aprendendo eletrônica do zero com Arduino. Cada projeto é pequeno, tem o código comentado em português, o desenho da montagem e um README explicando o que foi feito, por que e o que deu errado no caminho.

A ideia é que qualquer pessoa (inclusive eu daqui a um tempo) consiga refazer cada montagem só com o que está aqui.

## Projetos

Em ordem de dificuldade. Cada pasta tem `README.md`, o código `.ino` e um `montagem.html` com o desenho da ligação (abra no navegador).

| Projeto | O que faz | Aprende |
|---|---|---|
| [pisca_led](projetos/pisca_led/) | Pisca um LED e conta as piscadas pelo USB | Saída digital, resistor do LED, monitor serial |
| [lcd_ola](projetos/lcd_ola/) | Escreve "Ola, bancada!" no display e conta os segundos | Display LCD 16x2, biblioteca LiquidCrystal, contraste |
| [lcd_show](projetos/lcd_show/) | Show com animações no display e 5 LEDs acompanhando | Caracteres desenhados, PWM, várias saídas ao mesmo tempo |

## Material

O inventário completo, com desenhos interativos de cada peça, está em [`bancada.html`](bancada.html) (abra no navegador). Resumo:

- **Arduino Uno R3** (ATmega328P, lógica de 5 V)
- **Display LCD 16x2** modelo HA-1602: 14 pinos, sem luz de fundo, controlador compatível com HD44780
- **LEDs de 5 mm**: vermelhos, amarelos e verdes (~10)
- **Resistores** (~30), medidos com o multímetro:

  | Valor | Faixas de cor | Uso |
  |---|---|---|
  | 220 Ω ±1% | vermelho, vermelho, preto, preto, marrom | LED com brilho forte |
  | 470 Ω ±5% | amarelo, violeta, marrom, dourado | LED com brilho médio |
  | 1 kΩ ±5% | marrom, preto, vermelho, dourado | LED mais fraco, contraste do LCD |
  | 10 kΩ ±5% | marrom, preto, laranja, dourado | botões e sensores (pull-up) |
  | 100 kΩ ±5% | marrom, preto, amarelo, dourado | sensores; **não** serve para LED |

- **Protoboard** de 400 pontos
- **Multímetro digital** Thompson (estilo DT830)
- **Fios jumper** macho-macho
- A caminho: **ESP32** e um kit de componentes

Fotos de cada peça em [`fotos-componentes/`](fotos-componentes/).

## Regras de ouro

1. Nunca ligue 5V direto no GND (curto-circuito).
2. LED sempre com resistor em série.
3. Monte e mude fios com o USB desconectado.
4. Na dúvida sobre um resistor, meça com o multímetro: as cores enganam (violeta parece azul, e é fácil ler de trás para frente).

## Ferramentas

O código é compilado e enviado pelo terminal com o [arduino-cli](https://arduino.github.io/arduino-cli/). Instalação no macOS, uma vez só:

```bash
brew install arduino-cli
arduino-cli config init
arduino-cli core update-index
arduino-cli core install arduino:avr
arduino-cli lib install LiquidCrystal
```

Versões usadas: arduino-cli 1.6.0, `arduino:avr` 1.8.8, LiquidCrystal 1.0.7.

### Enviar um projeto para o Arduino

Conecte o Uno no USB e descubra a porta:

```bash
arduino-cli board list
```

Procure a linha `Arduino UNO` (no Mac é algo como `/dev/cu.usbmodem212201`). Depois, de dentro da pasta `projetos/`:

```bash
arduino-cli compile --fqbn arduino:avr:uno pisca_led
arduino-cli upload -p /dev/cu.usbmodem212201 --fqbn arduino:avr:uno pisca_led
```

Troque `pisca_led` pelo nome da pasta do projeto. O Arduino guarda **um programa por vez**: enviar um novo apaga o anterior.

Para ver as mensagens que o programa manda pelo USB:

```bash
arduino-cli monitor -p /dev/cu.usbmodem212201 -c baudrate=9600
```

### Editor

A pasta `.vscode/` configura o VS Code (extensão C/C++ da Microsoft) para reconhecer as funções do Arduino. Abra a pasta inteira, não só o arquivo:

```bash
code .
```

Sem isso o editor sublinha `pinMode`, `Serial` etc. em vermelho, mas o código compila normalmente.

## Estrutura

```
eletronica/
├── README.md               este arquivo
├── bancada.html            inventário interativo dos componentes
├── fotos-componentes/      fotos de cada peça
├── projetos/
│   ├── pisca_led/          README, código e montagem
│   ├── lcd_ola/
│   └── lcd_show/
└── .vscode/                configuração do editor para Arduino
```
