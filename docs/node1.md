12V is only connected to voltage regulator.


## UART <-> RS-232

| ATmega162   | <---> |           MAX233                 | <---> |  DSUB9   | --> PC
|             |       |                                  |       |          |
| RXD0 Pin 10 | <---> | R1OUT Pin 3 | <--- | T1OUT Pin 5 | <---> | RX Pin 2 |
| TXD0 Pin 11 | <---> | T1IN Pin 2  | ---> | R1IN Pin 4  | <---> | TX Pin 3 |

