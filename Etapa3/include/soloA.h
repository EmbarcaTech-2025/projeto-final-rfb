#ifndef SOLOA_H
#define SOLOA_H

#include <stdbool.h>

// Pino digital (D0) do sensor — ajustar conforme ligação
#define SENSOR_SOLO_PINO_DIGITAL_DO 16

// Inicializa o sensor de solo
void soloA_init(void);

// Lê o valor digital (true = seco, false = úmido)
bool soloA_ler_digital(void);

// Retorna texto "Seco" ou "Umido"
const char* soloA_status_solo(void);

#endif // SOLOA_H