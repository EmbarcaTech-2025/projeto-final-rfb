#include "include/soloA.h"
#include "hardware/gpio.h" // Adicionado para as funções gpio_init e gpio_get


void soloA_init(void) {
    gpio_init(SENSOR_SOLO_PINO_DIGITAL_DO);
    gpio_set_dir(SENSOR_SOLO_PINO_DIGITAL_DO, GPIO_IN);
    gpio_pull_down(SENSOR_SOLO_PINO_DIGITAL_DO); // evita flutuações se desconectar
}

bool soloA_ler_digital(void) {
    // HIGH = solo seco, LOW = solo úmido
    return gpio_get(SENSOR_SOLO_PINO_DIGITAL_DO);
}

const char* soloA_status_solo(void) {
    return soloA_ler_digital() ? "Seco" : "Umido";
}