# pisca_led

O primeiro projeto: faz um LED piscar a cada meio segundo e manda pelo USB quantas vezes já piscou.

Serve para testar a cadeia inteira (computador → arduino-cli → Arduino → LED) antes de montar coisas mais complexas.

## Material

- Arduino Uno + cabo USB
- 1 LED (qualquer cor)
- 1 resistor de **1 kΩ** (marrom, preto, vermelho, dourado). 220 Ω ou 470 Ω também servem, com o LED mais forte.
- Opcional: protoboard e 2 fios

Sem montar nada, o programa já pisca o LED **L** da própria placa, que fica ligado ao pino 13.

## Ligação

O desenho de cada passo está em [`montagem.html`](montagem.html). Há duas opções.

### Opção 1: sem fios, direto no Arduino

| De | Para |
|---|---|
| Perna longa (+) do LED | furo **13** |
| Perna curta (−) do LED | torcida junto com uma perna do resistor |
| Outra perna do resistor | furo **GND**, ao lado do 13 |

### Opção 2: na protoboard

| De | Para |
|---|---|
| Pino **13** do Arduino | furo **a10** (fio) |
| Perna longa (+) do LED | **d10** |
| Perna curta (−) do LED | **d11** |
| Resistor 1 kΩ | **a11** → trilho azul (−) |
| Trilho azul (−) | **GND** do Arduino, ao lado do 13 (fio) |

O caminho da corrente é: pino 13 → LED → resistor → GND. A ordem do LED e do resistor não importa; o que importa é o resistor estar no caminho para limitar a corrente.

## Como o código funciona

- `pinMode(13, OUTPUT)` diz que o pino 13 vai **mandar** sinal, e não ler.
- `digitalWrite(13, HIGH)` põe 5 V no pino (LED acende); `LOW` põe 0 V (apaga).
- `delay(500)` espera 500 milissegundos.
- `Serial.begin(9600)` abre a conversa com o computador a 9600 bits por segundo; `Serial.println(...)` manda uma linha de texto.

`setup()` roda uma vez quando a placa liga; `loop()` roda de novo sem parar.

## Enviar

De dentro de `projetos/`:

```bash
arduino-cli compile --fqbn arduino:avr:uno pisca_led
arduino-cli upload -p /dev/cu.usbmodem212201 --fqbn arduino:avr:uno pisca_led
arduino-cli monitor -p /dev/cu.usbmodem212201 -c baudrate=9600
```

O monitor deve mostrar:

```
Ola, bancada! O Uno ligou.
Piscadas: 1
Piscadas: 2
```

## O que aconteceu na prática

- **LED muito fraco:** foi usado por engano um resistor de 100 kΩ (marrom, preto, amarelo, dourado), que se confunde com o de 470 Ω (amarelo, violeta, marrom, dourado). No multímetro, na escala 20k, ele mostrava só "1", que quer dizer "passou do limite da escala". Trocado pelo de 1 kΩ, o LED acendeu normal.
- **LED apagado:** LED ao contrário não acende (nem queima). Vire: a perna longa vai no lado do pino 13.

## Conceitos

- **Ânodo (+) e cátodo (−):** a corrente só passa pelo LED num sentido. Perna longa = +; o lado chato da borda = −.
- **Lei de Ohm para o resistor do LED:** R = (5 V − tensão do LED) ÷ corrente. Para um LED vermelho (~2 V) com 1 kΩ: (5 − 2) ÷ 1000 = 3 mA.
