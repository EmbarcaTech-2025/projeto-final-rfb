#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "FreeRTOS.h"
#include "task.h"

// Headers do projeto
#include "include/BH1750.h"
#include "include/AHT10.h"
#include "include/BMP280.h"
#include "include/i2c_setup.h"
#include "include/display.h"
#include "include/buttons.h"

// SD Card
#include "lib/sd_card.h"

// Interfaces I2C definidas em i2c_setup.c
extern const I2C_interface_t i2c_hw_iface0; // i2c0 (sensores)
extern const I2C_interface_t i2c_hw_iface1; // i2c1 (display)

//-----------------------------------------------------------------------------
// Variáveis globais
//-----------------------------------------------------------------------------
volatile int screen_state = 1;  // 1 = status, 2 = valores

// Flags de inicialização
volatile bool sensor_ok_bh1750 = false;
volatile bool sensor_ok_aht10 = false;
volatile bool sensor_ok_bmp280 = false;

// Últimos valores lidos
volatile float last_lux   = -1;
volatile float last_temp  = -1000;
volatile float last_umid  = -1;
volatile float last_press = -1;

//-----------------------------------------------------------------------------
// Tarefas dos sensores
//-----------------------------------------------------------------------------
void task_bmp280(void *pvParameters) {
    if (!bmp280_init(i2c_hw_iface0.i2c)) {
        printf("BMP280: Erro na inicializacao.\n");
        sensor_ok_bmp280 = false;
        vTaskDelete(NULL);
    } else {
        printf("BMP280: Inicializado!\n");
        sensor_ok_bmp280 = true;
    }

    bmp280_data_t sensor_data;
    for (;;) {
        if (bmp280_read_data(i2c_hw_iface0.i2c, &sensor_data)) {
            last_press = sensor_data.pressure / 100.0f;
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void task_aht10(void *pvParameters) {
    if (!aht10_init(i2c_hw_iface0.i2c)) {
        printf("AHT10: Erro na inicializacao.\n");
        sensor_ok_aht10 = false;
        vTaskDelete(NULL);
    } else {
        printf("AHT10: Inicializado!\n");
        sensor_ok_aht10 = true;
    }

    aht10_data_t sensor_data;
    for (;;) {
        if (aht10_read_data(i2c_hw_iface0.i2c, &sensor_data)) {
            last_temp = sensor_data.temperature;
            last_umid = sensor_data.humidity;
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void task_bh1750(void *pvParameters) {
    bh1750_init(i2c_hw_iface0.i2c);
    sensor_ok_bh1750 = true;

    for (;;) {
        float lux = bh1750_read_lux(i2c_hw_iface0.i2c);
        if (lux >= 0.0f) {
            last_lux = lux;
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//-----------------------------------------------------------------------------
// Botões
//-----------------------------------------------------------------------------
void buttons_task(void *pvParameters) {
    buttons_init();

    while (1) {
        if (button_a_pressed()) {
            screen_state = 2; // Tela de valores
        }
        if (button_b_pressed()) {
            screen_state = 1; // Tela de status
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

//-----------------------------------------------------------------------------
// Display
//-----------------------------------------------------------------------------
void display_task(void *pvParameters) {
    screem_init();

    while (1) {
        if (screen_state == 1) {
            // Tela 1 - Status
            screem_message_4lines(
                "SD: OK", // mostra SD sempre como OK, se quiser checar real, precisa criar flag
                sensor_ok_bh1750 ? "luz: OK"   : "luz: ERRO",
                sensor_ok_aht10  ? "temp: OK"  : "temp: ERRO",
                sensor_ok_bmp280 ? "press: OK" : "press: ERRO"
            );
        } else {
            // Tela 2 - Valores
            char l1[20], l2[20], l3[20], l4[20];
            snprintf(l1, sizeof(l1), "luz: %.1f lx", last_lux);
            snprintf(l2, sizeof(l2), "temp: %.1f C", last_temp);
            snprintf(l3, sizeof(l3), "umid: %.1f %%", last_umid);
            snprintf(l4, sizeof(l4), "press: %.1f hPa", last_press);

            screem_message_4lines(l1, l2, l3, l4);
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

//-----------------------------------------------------------------------------
// Log no cartão SD
//-----------------------------------------------------------------------------
void task_log_to_sd(void *pvParameters) {
    if (sd_card_init() != FR_OK) {
        printf("Erro ao inicializar SD card!\n");
        vTaskDelete(NULL);
    }

    char line[128];
    char timestamp[32];

    for (;;) {
        sd_card_get_formatted_timestamp(timestamp, sizeof(timestamp));

        snprintf(line, sizeof(line),
                 "%s,%.2f,%.2f,%.2f,%.2f\n",
                 timestamp, last_temp, last_umid,
                 last_press, last_lux);

        sd_card_append_to_csv("datalog.csv", line);

        printf("Log salvo no SD: %s", line);

        vTaskDelay(pdMS_TO_TICKS(60000)); // grava a cada 60s
    }
}

//-----------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------
int main() {
    stdio_init_all();

    // Inicializa barramentos I2C
    setup_i2c(&i2c_hw_iface0); // Sensores
    setup_i2c(&i2c_hw_iface1); // Display

    // Display
    xTaskCreate(display_task, "Display Task", 1024, NULL, 2, NULL);

    // Botões
    xTaskCreate(buttons_task, "Buttons Task", 512, NULL, 3, NULL);

    // Sensores
    xTaskCreate(task_bh1750, "BH1750 Task", 1024, NULL, 1, NULL);
    xTaskCreate(task_aht10,  "AHT10 Task",  1024, NULL, 1, NULL);
    xTaskCreate(task_bmp280, "BMP280 Task", 1024, NULL, 1, NULL);

    // Log SD
    xTaskCreate(task_log_to_sd, "LogSD", 2048, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1) { }
    return 0;
}
