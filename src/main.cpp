#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "defs.h"

//
// task to blink the on-board LED
// this is to indicate basic liveness
void led_task(void *param)
{
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    while (true)
    {
        gpio_xor_mask(1 << PICO_DEFAULT_LED_PIN);
        vTaskDelay(pdMS_TO_TICKS(333));
    }
}

//
// main
int main()
{
    //
    // init stdin/stdout
    // waits a max of PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS (CmakeLists.txt) for
    // USB serial to connect to something
    stdio_init_all();
    //
    // init tasks
    xTaskCreate(led_task, "LED_Task", 256, NULL, 1, NULL);
    //
    // init valve controller
    // controller is running upon return
    init_valve_controller();

    recipe.open_time(5);
    recipe.close_time(10);

    command_initialize();

    //
    // run FreeRTOS scheduler - does not return
    vTaskStartScheduler();
    //
    // should never get here
    while (1)
    {
    };
}
