#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "hardware/spi.h"
#include <string.h> 
#include "hardware/i2c.h"
#include "semphr.h" 
#include "hardware/rtc.h"

#include "lib/sd_card.h"
#include "include/i2c_setup.h"
#include "include/AHT10.h"
#include "include/BH1750.h"
#include "include/BMP280.h"


// Interfaces I2C definidas em i2c_setup.c
extern const I2C_interface_t i2c_hw_iface0; // AHT10 e BMP280
extern const I2C_interface_t i2c_hw_iface1; // BH1750


// Protótipos das tasks
void task_aht10(void *pvParameters);
void task_bh1750(void *pvParameters);
void task_bmp280(void *pvParameters);
void task_logger(void *pvParameters);

// Estrutura compartilhada para armazenar leituras
typedef struct {
    float temperature;
    float humidity;
    float lux;
    float pressure;
} sensor_data_t;

static sensor_data_t g_sensor_data;
static SemaphoreHandle_t xDataMutex;


void task_aht10(void *pvParameters) {
    aht10_data_t data;
    for (;;) {
        if (aht10_read_data(i2c_hw_iface0.i2c, &data)) {
            xSemaphoreTake(xDataMutex, portMAX_DELAY);
            g_sensor_data.temperature = data.temperature;
            g_sensor_data.humidity = data.humidity;
            xSemaphoreGive(xDataMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void task_bh1750(void *pvParameters) {
    for (;;) {
        float lux = bh1750_read_lux(i2c_hw_iface1.i2c);
        if (lux >= 0) {
            xSemaphoreTake(xDataMutex, portMAX_DELAY);
            g_sensor_data.lux = lux;
            xSemaphoreGive(xDataMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void task_bmp280(void *pvParameters) {
    bmp280_data_t data;
    for (;;) {
        if (bmp280_read_data(i2c_hw_iface0.i2c, &data)) {
            xSemaphoreTake(xDataMutex, portMAX_DELAY);
            g_sensor_data.pressure = data.pressure / 100.0f;
            xSemaphoreGive(xDataMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void task_logger(void *pvParameters) {
    char line[128];
    char timestamp[32];

    // Inicializa cartão SD
    if (sd_card_init() != FR_OK) {
        printf("Erro ao inicializar o cartão SD!\n");
        vTaskDelete(NULL);
    }

    for (;;) {
        xSemaphoreTake(xDataMutex, portMAX_DELAY);
        float temp = g_sensor_data.temperature;
        float hum  = g_sensor_data.humidity;
        float lux  = g_sensor_data.lux;
        float pres = g_sensor_data.pressure;
        xSemaphoreGive(xDataMutex);

        // Timestamp
        sd_card_get_formatted_timestamp(timestamp, sizeof(timestamp));

        snprintf(line, sizeof(line),
                 "%s,%.2f,%.2f,%.2f,%.2f\n",
                 timestamp, temp, hum, lux, pres);

        printf("%s", line); // Mostra no terminal também

        sd_card_append_to_csv("dados.csv", line);

        vTaskDelay(pdMS_TO_TICKS(5000)); // grava a cada 5s
    }
}

int main() {
    stdio_init_all();
    sleep_ms(2000);

    // Inicializa I2C
    setup_i2c(&i2c_hw_iface0);
    setup_i2c(&i2c_hw_iface1);


    // Inicializa sensores
    if (!aht10_init(i2c_hw_iface0.i2c)) {
        printf("ERRO: Falha ao inicializar AHT10\n");
        while (1);
    }
    bh1750_init(i2c_hw_iface1.i2c);
    bmp280_init(i2c_hw_iface0.i2c);
    

    // Inicializa RTC com data/hora default (exemplo)
    datetime_t t = { .year = 2025, .month = 8, .day = 29, .dotw = 5,
                     .hour = 23, .min = 0, .sec = 0 };
    rtc_init();
    rtc_set_datetime(&t);

    // Mutex para proteger dados globais
    xDataMutex = xSemaphoreCreateMutex();

    // Cria as tasks
    xTaskCreate(task_aht10, "AHT10", 1024, NULL, 1, NULL);
    xTaskCreate(task_bh1750, "BH1750", 1024, NULL, 1, NULL);
    xTaskCreate(task_bmp280, "BMP280", 1024, NULL, 1, NULL);
    xTaskCreate(task_logger, "Logger", 2048, NULL, 1, NULL);

    // Inicia o scheduler
    vTaskStartScheduler();

    while (true) {
        tight_loop_contents();
    }
    return 0;
}

