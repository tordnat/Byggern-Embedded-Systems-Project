12V is only connected to voltage regulator.


## UART <-> RS-232
```
| ATmega162   | <---> |           MAX233                 | <---> |  DSUB9   | --> PC   
|             |       |                                  |       |          |   
| RXD0 Pin 10 | <---> | R1OUT Pin 3 | <--- | T1OUT Pin 5 | <---> | RX Pin 2 |   
| TXD0 Pin 11 | <---> | T1IN Pin 2  | ---> | R1IN Pin 4  | <---> | TX Pin 3 |   
```
## External Memory SRAM

SRAM address pins used:
- A0-A3

## ADC
```
Joystick
Pin1(AXIS1 ) -> AIN0
Pin2 (AXIS2) -> AIN1
Pin3 (BUTTON) -> AIN2
  
```

## Filter

T_meas = 3.5us
T = 0.0002s = 200us
f_cutoff (-3dB) = 796Hz
f_cutoff = 1/1/(2*pi*3.5*10^(-6)) Hz ≈ 45472 Hz = 4.5kHz
Slew rate = -20 dB/decade standard
