#include "include/air.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include <stdio.h>

// Comandos do AHT10
#define AHT10_CMD_INIT        0xE1
#define AHT10_CMD_TRIGGER     0xAC
#define AHT10_STATUS_BUSY     0x80

void air_init(void) {
    // Inicializa I2C
    i2c_init(AHT10_I2C, 100 * 1000);
    gpio_set_function(AHT10_SDA, GPIO_FUNC_I2C);
    gpio_set_function(AHT10_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(AHT10_SDA);
    gpio_pull_up(AHT10_SCL);

    // Envia comando de inicialização
    uint8_t cmd[3] = {AHT10_CMD_INIT, 0x08, 0x00};
    i2c_write_blocking(AHT10_I2C, AHT10_ADDR, cmd, 3, false);
    sleep_ms(50); // tempo de estabilização
}

bool air_read(float* temperature, float* humidity) {
    uint8_t data[6];
    uint8_t cmd = AHT10_CMD_TRIGGER;

    // Dispara medição
    i2c_write_blocking(AHT10_I2C, AHT10_ADDR, &cmd, 1, false);
    sleep_ms(120); // tempo de medição

    // Lê os 6 bytes do sensor
    if (i2c_read_blocking(AHT10_I2C, AHT10_ADDR, data, 6, false) < 0) return false;

    // Verifica se sensor está ocupado
    if (data[0] & AHT10_STATUS_BUSY) return false;

    // Calcula umidade (20 bits)
    uint32_t raw_hum = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | ((data[3] & 0xF0) >> 4);
    *humidity = ((float)raw_hum * 100.0f / 1048576.0f);

    // Calcula temperatura (20 bits)
    uint32_t raw_temp = ((uint32_t)(data[3] & 0x0F) << 16) | ((uint32_t)data[4] << 8) | data[5];
    *temperature = ((float)raw_temp * 200.0f / 1048576.0f) - 50.0f;

    return true;
}

float air_get_temperature(void) {
    float t, h;
    if (!air_read(&t, &h)) return -1000.0f;
    return t;
}

float air_get_humidity(void) {
    float t, h;
    if (!air_read(&t, &h)) return -1.0f;
    return h;
}