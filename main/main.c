#include <stdio.h>
#include "esp_adc/adc_oneshot.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "adcCon.h"



void app_main(void)
{


xTaskCreate(readADC,"readADC",4096,NULL,5,NULL);



}