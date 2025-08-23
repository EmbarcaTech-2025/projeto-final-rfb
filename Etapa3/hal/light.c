#include "include/light.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"

// Endereço I2C padrão do BH1750
#define BH1750_ADDR 0x23

// Comandos do BH1750
#define BH1750_POWER_ON   0x01
#define BH1750_RESET      0x07
#define BH1750_CONT_HRES  0x10

void light_init() {
    // Inicializa I2C nos pinos definidos em light.h
    i2c_init(BH1750_I2C, 100 * 1000);
    gpio_set_function(BH1750_SDA, GPIO_FUNC_I2C);
    gpio_set_function(BH1750_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(BH1750_SDA);
    gpio_pull_up(BH1750_SCL);

    // Liga o sensor
    uint8_t cmd = BH1750_POWER_ON;
    i2c_write_blocking(BH1750_I2C, BH1750_ADDR, &cmd, 1, false);

    // Reseta o sensor
    cmd = BH1750_RESET;
    i2c_write_blocking(BH1750_I2C, BH1750_ADDR, &cmd, 1, false);

    // Configura para modo contínuo de alta resolução
    cmd = BH1750_CONT_HRES;
    i2c_write_blocking(BH1750_I2C, BH1750_ADDR, &cmd, 1, false);
}

float light_read_lux() {
    uint8_t data[2];
    if (i2c_read_blocking(BH1750_I2C, BH1750_ADDR, data, 2, false) < 0) {
        return -1.0f;
    }
    uint16_t raw = (data[0] << 8) | data[1];
    return raw / 1.2f;  // Conversão conforme datasheet
}