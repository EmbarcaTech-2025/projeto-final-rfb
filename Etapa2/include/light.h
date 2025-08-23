#ifndef LIGHT_H
#define LIGHT_H

#include <stdint.h>

// Pinos I2C para o BH1750 (ajuste conforme seu hardware)
#define BH1750_I2C i2c0
#define BH1750_SDA 0
#define BH1750_SCL 1

// Inicializa o sensor BH1750
void light_init();

// Retorna luminosidade em lux (ou -1 em caso de erro)
float light_read_lux();

#endif