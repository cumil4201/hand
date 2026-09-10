#include <stdio.h>
#include "esp_adc/adc_oneshot.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "adcCon.h"

#define ADC_CHANNEL ADC_CHANNEL_4
//static const char *TAG = "main";

adc_oneshot_unit_handle_t adc_handler;
adc_oneshot_unit_init_cfg_t adc_config1 = {
    .unit_id = ADC_UNIT_1,
    .ulp_mode = ADC_ULP_MODE_DISABLE,
};
adc_oneshot_chan_cfg_t adc_chann_config1 = {
    .atten = ADC_ATTEN_DB_12,
    .bitwidth = ADC_BITWIDTH_DEFAULT

};

int value =0;
void app_main(void)
{
    /*adc_oneshot_new_unit(&adc_config1, &adc_handler);
    adc_oneshot_config_channel(adc_handler, ADC_CHANNEL, &adc_chann_config1);
    while (1)
    {
        adc_oneshot_read(adc_handler,ADC_CHANNEL,&value);
        printf("%d \n",value);

        vTaskDelay(20 / portTICK_PERIOD_MS);

    } */

xTaskCreate(readADC,"readADC",4096,NULL,5,NULL);



}