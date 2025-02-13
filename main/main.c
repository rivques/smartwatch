#include <stdio.h>
#include "display_handler.h"
#include "freertos/FreeRTOS.h"

void app_main(void)
{
    display_handler_init();
    demo();
    while (1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    
}