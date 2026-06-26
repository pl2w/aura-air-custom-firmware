| Component                  | Pin(s) | Details                                                |
| -------------------------- | ------ | ------------------------------------------------------ |
| HDC1080 (Temp/Humidity)    | D0, D1 | I2C address 0x40                                       |
| SGP30 (TVOC/eCO2)          | D0, D1 | I2C address 0x58 (shared bus)                          |
| ZPH02 PM2.5                | D2, D3 | UART 9600 baud                                         |
| ME2CO (Carbon Monoxide)    | A2     | Analog input                                           |
| Ionizer (IS1-12D5-S1B)     | D2     | GPIO output, HIGH=on (shared with ZPH02 RX)            |
| Fan (PB3N175B12H)          | 24     | PWM output, 0-80 (lower = faster)                      |
| Fan FG (tachometer)        | 27     | Input, 2 pulses/rev, FALLING interrupt                 |
| UVC LEDs                   | 25     | GPIO output, HIGH=on                                   |
| Cover switch               | 28     | GPIO input, LOW=closed, HIGH=open                      |m