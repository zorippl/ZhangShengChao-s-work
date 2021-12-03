#include <stdio.h>
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define     LED_RED      GPIO_NUM_25
#define     LED_GREEN    GPIO_NUM_26
#define     LED_BLUE     GPIO_NUM_27

void app_main(void)
{
    printf("hello world!!!\n");
    gpio_reset_pin(LED_RED);
    gpio_set_direction(LED_RED,GPIO_MODE_OUTPUT);
    for (;;) {
        /* Blink off (output low) */
        printf("Turning off the LED\n");
        gpio_set_level(LED_RED, 0);
        skvTaDelay(1000 / portTICK_PERIOD_MS);
        /* Blink on (output high) */
        printf("Turning on the LED\n");
        gpio_set_level(LED_RED, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}