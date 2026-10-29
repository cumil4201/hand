#include <stdio.h>                              
#include "freertos/FreeRTOS.h"                  
#include "freertos/task.h"                      
#include "esp_adc/adc_continuous.h"             
#include "esp_log.h"
//Header
#include "adcCon.h"
#include "servo.h"

#define ADC_CHANNEL ADC_CHANNEL_6 //gpio 34
#define FRAME_SIZE 256
#define SAMPLE_FREQ 20000

static char* TAG = "adcContinous";
static bool clinching = 0;
static uint32_t avg_filtr_res = 0;

void readADC(void *pvParameters){

adc_continuous_handle_t handle = NULL;

adc_continuous_handle_cfg_t handle_config = {
    .max_store_buf_size = 4096,
    .conv_frame_size = FRAME_SIZE
};

adc_continuous_new_handle(&handle_config,&handle);


adc_digi_pattern_config_t pattern_config = {
.atten = ADC_ATTEN_DB_12,
.channel = ADC_CHANNEL,
.bit_width = SOC_ADC_DIGI_MAX_BITWIDTH,
.unit = ADC_UNIT_1
};

adc_continuous_config_t adc_con_config = {
    .adc_pattern = &pattern_config,
    .conv_mode = ADC_CONV_SINGLE_UNIT_1,
    .format = ADC_DIGI_OUTPUT_FORMAT_TYPE1,
    .pattern_num = 1,
    .sample_freq_hz = SAMPLE_FREQ
};


adc_continuous_config(handle,&adc_con_config);


uint8_t res[FRAME_SIZE];
uint32_t retur_num; 

adc_continuous_start(handle);
while (1)
{
    
    esp_err_t resultAdcRead= adc_continuous_read(handle,res,FRAME_SIZE,&retur_num,pdMS_TO_TICKS(1000));
    
if(resultAdcRead == ESP_OK){

uint32_t sum = 0;
uint32_t amout_sampl = retur_num/SOC_ADC_DIGI_RESULT_BYTES;


for(int i = 0 ; i < retur_num ; i += SOC_ADC_DIGI_RESULT_BYTES){
adc_digi_output_data_t *p = (adc_digi_output_data_t*) &res[i];
sum += p->type1.data;

//printf("%d\n",p->type1.data);


}

if(amout_sampl > 0){
    avg_filtr_res = sum/amout_sampl;
    //printf("%ld \n",avg_filtr_res);
    // calculate the difference !!!

    //if(diff > value ){ clinching = ! clinching  }

    //servo_set_pos(clinching);

    servo_set_pos(avg_filtr_res);
}


}
vTaskDelay(1);

}

};
