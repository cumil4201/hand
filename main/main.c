#include <stdio.h>
#include "esp_adc/adc_oneshot.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "adcCon.h"
#include "servo.h"



void app_main(void)
{

servo_init();
xTaskCreate(readADC,"readADC",4096,NULL,5,NULL);



}