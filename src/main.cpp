#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "utils.h"

#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#if defined(CYW43_WL_GPIO_LED_PIN)
#define LED_PIN CYW43_WL_GPIO_LED_PIN
#elif defined(PICO_DEFAULT_LED_PIN)
#define LED_PIN PICO_DEFAULT_LED_PIN
#else
#error no default pin defined
#endif

#ifndef LED_ON_MS
#error LED_ON_MS not defined - define in CMakeLists.txt
#endif
#ifndef LED_OFF_MS
#error LED_OFF_MS not defined - define in CMakeLists.txt
#endif

void led_task(void *param)
{
#if defined(CYW43_WL_GPIO_LED_PIN)
    cyw43_arch_init();
#else
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
#endif

    while (true)
    {
#if defined(PICO_DEFAULT_LED_PIN)
        gpio_put(LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(LED_ON_MS));
        gpio_put(LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(LED_OFF_MS));
#elif defined(CYW43_WL_GPIO_LED_PIN)
        cyw43_arch_gpio_put(LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(LED_ON_MS));
        cyw43_arch_gpio_put(LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(LED_OFF_MS));
#endif
    }
}

int main()
{

    stdio_init_all();

    printf("-- starting  pin=%d\n", LED_PIN);
    printf("-- persistent store: %d\n", getPersistentStoreSize());
    sleep_ms(500);
    xTaskCreate(led_task, "LED_Task", 256, NULL, 1, NULL);
    printf("-- running\n");
    vTaskStartScheduler();

    while (1)
    {
    };
}
