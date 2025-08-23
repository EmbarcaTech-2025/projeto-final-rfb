#ifndef DISPLAY_H
#define DISPLAY_H

#include "../drivers/ssd1306.h"
#include "hardware/i2c.h"
#include <string.h>
#include "pico/stdlib.h" 

#define I2C_SDA 14
#define I2C_SCL 15
#define I2C_PORT i2c1
#define SSD1306_ADDR 0x3C

void screen_init();

void screen_message(const char* msg);
void screen_message_multiline(const char* msg);
void screen_centered_message(const char* msg);
void screen_message_4lines(const char* l1, const char* l2, const char* l3, const char* l4);

#endif 