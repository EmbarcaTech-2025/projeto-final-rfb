#include "include/soloB.h"
#include "hardware/adc.h"
#include "pico/stdlib.h"

/**
 * Inicializa o ADC e seleciona o canal do sensor
 */
void soloB_init(void) {
    adc_init();
    adc_gpio_init(SENSOR_SOLO_CAPACITIVO_ADC_GPIO);
    adc_select_input(SENSOR_SOLO_CAPACITIVO_ADC_CH);
}

/**
 * Lê valor bruto do ADC
 */
uint16_t soloB_read_raw(void) {
    return adc_read(); // valor entre 0 e 4095
}

uint16_t soloB_read_raw_avg(uint8_t n) {
    uint32_t soma = 0;
    for (uint8_t i = 0; i < n; i++) {
        soma += adc_read();
        sleep_ms(5);
    }
    return (uint16_t)(soma / n);
}

/**
 * Converte valor bruto em porcentagem
 */
float soloB_read_percent(uint16_t valor_bruto) {
    if (valor_bruto > SOLO_VALOR_SECO) valor_bruto = SOLO_VALOR_SECO;
    if (valor_bruto < SOLO_VALOR_MOLHADO) valor_bruto = SOLO_VALOR_MOLHADO;

    float porcentagem = ((float)(SOLO_VALOR_SECO - valor_bruto) / 
                        (SOLO_VALOR_SECO - SOLO_VALOR_MOLHADO)) * 100.0f;
    return porcentagem;
}
