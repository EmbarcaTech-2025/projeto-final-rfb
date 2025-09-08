#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "FreeRTOS.h"
#include "task.h"
#include "thingspeak.h"

// Headers do projeto
#include "include/BH1750.h"
#include "include/AHT10.h"
#include "include/BMP280.h"
#include "include/i2c_setup.h"
#include "include/display.h"
#include "include/buttons.h"
#include "include/thingspeak.h"

// SD Card
#include "lib/sd_card.h"

// WI-FI
#include "include/credentials.h"
#include "pico/cyw43_arch.h"
#include "lwip/netif.h"
#include "lwip/dhcp.h"
#include "lwip/ip4_addr.h"

// Interfaces I2C definidas em i2c_setup.c
extern const I2C_interface_t i2c_hw_iface0; // sensoress
extern const I2C_interface_t i2c_hw_iface1; // display

//-----------------------------------------------------------------------------
// Variáveis globais
//-----------------------------------------------------------------------------
volatile int screen_state = 1; // 1=status, 2=valores, 3=alertas

// Flags de inicialização
volatile bool sensor_ok_bh1750 = false;
volatile bool sensor_ok_aht10  = false;
volatile bool sensor_ok_bmp280 = false;

// Últimos valores lidos
volatile float last_lux   = -1;
volatile float last_temp  = -1000;
volatile float last_umid  = -1;
volatile float last_press = -1;

// Alertas
typedef struct {
    char message[32];
    bool active;
} Alert_t;

Alert_t current_alert = {"", false};

// Wifi
// Status Wi-Fi
volatile bool wifi_connected = false;
char wifi_ip[20] = "0.0.0.0";


//-----------------------------------------------------------------------------
// Função para checar alertas
//-----------------------------------------------------------------------------
void check_alerts() {
    current_alert.active = false;
    const char* msg = NULL;

    if (last_temp < 5.0f) msg = "Risco de geada";
    else if (last_temp > 35.0f) msg = "Calor excessivo";
    else if (last_umid < 30.0f) msg = "Umidade baixa";
    else if (last_umid > 85.0f) msg = "Risco de fungos";
    else if (last_lux < 200.0f) msg = "Luz insuficiente";
    else if (last_lux > 50000.0f) msg = "Sol intenso";
    else if (last_press < 1000.0f) msg = "Tendencia de chuva";
    else if (last_press > 1020.0f) msg = "Tempo estavel";

    if (msg != NULL) {
        snprintf(current_alert.message, sizeof(current_alert.message), "%s", msg);
        current_alert.active = true;
    }
}

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
            screen_state++;
            if (screen_state > 4) screen_state = 1;
        }
        if (button_b_pressed()) {
            screen_state = 1; // sempre volta para status
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
        check_alerts();

        if (screen_state == 1) {
            screem_message_4lines(
                "SD: OK",
                sensor_ok_bh1750 ? "luz: OK"   : "luz: ERRO",
                sensor_ok_aht10  ? "temp: OK"  : "temp: ERRO",
                sensor_ok_bmp280 ? "press: OK" : "press: ERRO"
            );
        } else if (screen_state == 2) {
            char l1[20], l2[20], l3[20], l4[20];
            snprintf(l1, sizeof(l1), "luz: %.1f lx", last_lux);
            snprintf(l2, sizeof(l2), "temp: %.1f C", last_temp);
            snprintf(l3, sizeof(l3), "umid: %.1f %%", last_umid);
            snprintf(l4, sizeof(l4), "press: %.1f hPa", last_press);
            screem_message_4lines(l1, l2, l3, l4);
        } else if (screen_state == 3) {
            if (current_alert.active) {
                screem_message_4lines("ALERTAS!", current_alert.message, "", "");
            } else {
                screem_message_4lines("ALERTAS!", "Nenhum alerta", "", "");
            }
        } else if (screen_state == 4) {
            if (wifi_connected) {
                char l1[20], l2[20];
                snprintf(l1, sizeof(l1), "Wi-Fi: OK");
                snprintf(l2, sizeof(l2), "IP: %.15s", wifi_ip);
                screem_message_4lines(l1, l2, SSID, "");
            } else {
                screem_message_4lines("Wi-Fi: OFF", "Sem conexao", "", "");
            }
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
        vTaskDelay(pdMS_TO_TICKS(60000));
    }
}

//-----------------------------------------------------------------------------
// Conectar WI-FI
//------------------------------------------------------------------------------
void wifi_task(void *pvParameters) {
    if (cyw43_arch_init()) {
        printf("Falha ao inicializar Wi-Fi\n");
        wifi_connected = false;
        vTaskDelete(NULL);
    }
    cyw43_arch_enable_sta_mode();

    printf("Conectando ao Wi-Fi %s...\n", SSID);
    if (cyw43_arch_wifi_connect_timeout_ms(SSID, PASSWORD,
                                           CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("Falha ao conectar ao Wi-Fi\n");
        wifi_connected = false;
        vTaskDelete(NULL);
    } else {
        printf("Wi-Fi conectado!\n");
        wifi_connected = true;

        // Mostra o IP obtido
        struct netif *netif = netif_default;
        if (netif != NULL) {
            snprintf(wifi_ip, sizeof(wifi_ip), "%s", ip4addr_ntoa(netif_ip4_addr(netif)));
            printf("IP obtido: %s\n", wifi_ip);
        }
    }

    while (1) {
        printf("Temp: %.2f | Umid: %.2f | Press: %.2f | Lux: %.2f\n",
               last_temp, last_umid, last_press, last_lux);
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}

void thingspeak_task(void *params)
{
    (void)params;
    while (!wifi_connected) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    TickType_t last_wake = xTaskGetTickCount();

    for (;;) {
        if (sensor_ok_aht10 && sensor_ok_bmp280 && sensor_ok_bh1750) {
            printf("Enviando dados para ThingSpeak...\n");
            thingspeak_send(
                WRITE_API_KEY,
                4,
                last_temp,
                last_umid,
                last_press,
                last_lux
            );
            printf("Status sensores: BH1750=%d, AHT10=%d, BMP280=%d\n",
                   sensor_ok_bh1750, sensor_ok_aht10, sensor_ok_bmp280);
        }
        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(THINGSPEAK_SEND_PERIOD_S * 1000));
    }
}
//-----------------------------------------------------------------------------
// Main
//------------------------------------------------------------------------------
int main() {
    stdio_init_all();

    // Inicializa I2C
    setup_i2c(&i2c_hw_iface0);
    setup_i2c(&i2c_hw_iface1);

    // Tarefas
    xTaskCreate(display_task, "Display Task", 1024, NULL, 2, NULL);
    xTaskCreate(buttons_task, "Buttons Task", 512, NULL, 3, NULL);
    xTaskCreate(task_bh1750, "BH1750 Task", 1024, NULL, 1, NULL);
    xTaskCreate(task_aht10,  "AHT10 Task", 1024, NULL, 1, NULL);
    xTaskCreate(task_bmp280, "BMP280 Task", 1024, NULL, 1, NULL);
    xTaskCreate(task_log_to_sd, "LogSD", 2048, NULL, 1, NULL);
    xTaskCreate(wifi_task, "WiFi Task", 4096, NULL, 2, NULL);
    xTaskCreate(thingspeak_task, "ThingSpeak Task", 2048, NULL, 2, NULL);

    vTaskStartScheduler();

    while (1) {}
    return 0;
}
