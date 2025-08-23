#ifndef AIR_H
#define AIR_H

#include <stdint.h>
#include <stdbool.h>
#include "hardware/i2c.h"

// Pinagem e endereço I2C
#define AHT10_I2C i2c1
#define AHT10_SDA 2
#define AHT10_SCL 3
#define AHT10_ADDR 0x38  // ou 0x39 dependendo do módulo

// Inicializa o AHT10
void air_init(void);

// Lê temperatura em °C (retorna -1000.0 em caso de erro)
float air_get_temperature(void);

// Lê umidade relativa em % (retorna -1.0 em caso de erro)
float air_get_humidity(void);

// Lê ambos valores juntos, retorna true se sucesso
bool air_read(float* temperature, float* humidity);

#endif // AIR_H