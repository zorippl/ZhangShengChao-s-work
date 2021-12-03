#include <stdio.h>
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define     LED_RED      GPIO_NUM_25
#define     LED_GREEN    GPIO_NUM_26
#define     LED_BLUE     GPIO_NUM_27

// 任务句柄
TaskHandle_t task1_handle,task2_handle;

// 任务函数
void task1_func(void* params);
void task2_func(void* params);

static void task(void* pvPar)
{
    while (1) {
        printf("I'm %s\r\n", (char *)pvPar);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    printf("hello world!!!\n");
    gpio_reset_pin(LED_RED);
    gpio_set_direction(LED_RED,GPIO_MODE_OUTPUT);
    xTaskCreatePinnedToCore(
        task1_func,         /*任务函数*/
        "task1",            /*任务名称*/
        2048,               /*任务堆栈大小*/
        NULL,               /*任务参数*/
        1,                  /*任务优先级*/
        &task1_handle,      /*任务句柄*/
        0                   /*处理器ID,即指定的CPU*/
        );
    xTaskCreatePinnedToCore(
        task2_func,         /*任务函数*/
        "task2",            /*任务名称*/
        2048,               /*任务堆栈大小*/
        NULL,               /*任务参数*/
        1,                  /*任务优先级*/
        &task2_handle,      /*任务句柄*/
        1                   /*处理器ID,即指定的CPU*/
        );
}

void task1_func(void* params){
    for(;;){
        /* Blink off (output low) */
            printf("Turning off the LED\n");
        gpio_set_level(LED_RED, 0);
        vTaskDelay(500 / portTICK_RATE_MS);   
            }
}

    
void task2_func(void* params){
    for(;;){
        /* Blink on (output high) */
        printf("Turning on the LED\n");
            gpio_set_level(LED_RED, 1);
        vTaskDelay(2000 / portTICK_RATE_MS);
    }
}



