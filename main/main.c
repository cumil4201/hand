#include <stdio.h>
#include "esp_adc/adc_oneshot.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"


#include "adcCon.h"
#include "servo.h"
#include "IMU.h"



void app_main(void)
{
gpio_install_isr_service(0);
servo_init();
imu_init();
xTaskCreate(readADC,"readADC",4096,NULL,5,NULL);
while (1)
{
    vTaskDelay(100 / portTICK_PERIOD_MS);
    
}



}