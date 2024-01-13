# RPI_PICO_PIO_KNX
Raspberry Pi Pico (RP2040) PIO KNX Example

### Features

- Short example howto use the PIO peripherie from the RP2040 (rpi pico) to communicate with KNX (via Transmitter like STKNX)
- KNX based on UART with 9600 Baud, 8 bit + even parity and one stop bit. Line coding is inverse and a return to zero coding with an duty of 33%.
- From the PIO driver's point of view there will transmit an 9N1 UART
- Parity bit is not calculated inside PIO and must be integrated in transmit function. The same applies to RX
- This example is able to use all GPIO Pins for Tx and Rx.
- PIO TX Statemachine includes a simple CSMA/CA which  abrod the commuinication if logic level on tx and rx are not the same.  IRQ Flag 0 will be set and wait until the flag is cleared
- PIO RX Statemachine contains a simple check that checks the status of the stop bit after 9 bits. If not ok, IRQ 1 is set and wait until the flag is cleared