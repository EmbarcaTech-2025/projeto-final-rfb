#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "include/display.h"
#include "include/soloA.h"
#include "include/soloB.h"
#include "include/light.h"
#include "include/air.h"

int main(void) {
    stdio_init_all();
    screen_init();
    soloA_init();
    soloB_init();
    light_init();


    char linha1[32], linha2[32], linha3[32], linha4[32];

    while (true) {
         // Leitura dos sensores de solo
        const char* status_soloA = soloA_status_solo();
        uint16_t valorB = soloB_read_raw_avg(30);
        float umidadeB = soloB_read_percent(valorB);

        // Luminosidade
        float lux = light_read_lux();

        // Ar
    float temp, hum;
    if (!air_read(&temp, &hum)) {
        temp = -1000.0;
        hum = -1.0;
    }
        sleep_ms(20);

        // Monta texto para display
        sprintf(linha1, "SoloA: %s", status_soloA);
        sprintf(linha2, "SoloB: %.1f%%", umidadeB);
        sprintf(linha3, "Lux: %.1f", lux);
        sprintf(linha4, "T:%.1fC U:%.1f%%", temp, hum);

        // Serial debug
        printf("%s | %s | %s | %s\n", linha1, linha2, linha3, linha4);
        sleep_ms(20);

        // OLED
        screen_message_4lines(linha1, linha2, linha3, linha4);

        sleep_ms(2000);

    }
}