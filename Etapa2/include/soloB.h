#ifndef SOLOB_H
#define SOLOB_H

#include <stdint.h>


// GPIO e canal ADC do sensor capacitivo
#define SENSOR_SOLO_CAPACITIVO_ADC_GPIO 28
#define SENSOR_SOLO_CAPACITIVO_ADC_CH   2

// Valores de calibração — ajuste conforme seu sensor
#define SOLO_VALOR_SECO  2100  
#define SOLO_VALOR_MOLHADO 2000 

void soloB_init(void);
uint16_t soloB_read_raw(void);
float soloB_read_percent(uint16_t valor_bruto);
uint16_t soloB_read_raw_avg(uint8_t n);  // ← nova função

#endif // SOLOB_H