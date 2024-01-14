#include <Arduino.h>

// put function declarations here:
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "knx_uart_pio.h"

#define KNX_RX_PIN  0
#define KNX_TX_PIN  1

#define KNX_BAUD 9600

#define rx_buffer_size 250
char rx_buffer[rx_buffer_size];
uint16_t rx_prt = 0;

void init_knx_uart(){

    uint offset_tx = pio_add_program(pio0, &knx_tx_program);
    uint offset_rx = pio_add_program(pio0, &knx_rx_program);

    pio_sm_set_pins_with_mask(pio0, 0, 0, 1u << KNX_TX_PIN);
    pio_sm_set_pindirs_with_mask(pio0, 0, 1u << KNX_TX_PIN , 1u << KNX_TX_PIN | 1u << KNX_RX_PIN);
    pio_sm_set_pindirs_with_mask(pio0, 1, 0, 1u << KNX_RX_PIN);

    pio_gpio_init(pio0, KNX_TX_PIN);
    pio_gpio_init(pio0, KNX_RX_PIN);

    gpio_pull_down(KNX_RX_PIN);

    knx_rx_program_init(pio0, 1, offset_rx, KNX_RX_PIN, KNX_BAUD);
    knx_tx_program_init(pio0, 0, offset_tx, KNX_TX_PIN, KNX_RX_PIN, KNX_BAUD);
}

char* transmit_knx(char * str){
    rx_prt = 0;
    while (*str){
        
        uint8_t ok = 1;
        knx_tx_program_putc(pio0, 0, *str++);
        while(pio_sm_is_tx_fifo_full(pio0, 0)){
          tight_loop_contents() ;
          if( knx_tx_get_irq(pio0, 0)){
            knx_tx_clr_irq(pio0, 0);
            Serial.println("IRQ TX Rised");
            ok = 0;
            break;
          } 
        }
        if(ok != 1){
          break;
        }

        char dummy = knx_rx_program_get_char(pio0, 1);
        if(dummy != 0x00){
          rx_buffer[rx_prt++] = dummy;
        }
        else{
          Serial.println("Parity not match");
        }
        
        
    }
    return  &rx_buffer[0];
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  sleep_ms(1000);
  Serial.println("KNX PIO Test");
  sleep_ms(1000);
  init_knx_uart();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Hello World");
  char * str = transmit_knx("Hello KNX World");

  Serial.println(str);
  
  sleep_ms(500);
}


