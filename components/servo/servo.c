#include "servo.h"
#include "driver/ledc.h"
#define LEDC_GPIO 2 
static bool is_clenched = 0;
static int present_state = -1;
void servo_init()
{
    
    ledc_timer_config_t ledc_config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 50,
        .duty_resolution = LEDC_TIMER_13_BIT,
        .clk_cfg = LEDC_AUTO_CLK};
        
        ledc_timer_config(&ledc_config);
        ledc_channel_config_t channel_config ={
            .gpio_num = LEDC_GPIO,
            .speed_mode = LEDC_LOW_SPEED_MODE,
            .channel = LEDC_CHANNEL_0,
            .intr_type = LEDC_INTR_DISABLE,
            .timer_sel = LEDC_TIMER_0,
            .duty = 200,
            .hpoint = 0
        };
        ledc_channel_config(&channel_config);
        ledc_fade_func_install(1);
}


void servo_set_pos(uint32_t adc_sample){
uint16_t min_pwm = 250;
uint16_t max_pwm = 980;
uint16_t duty = 0;






if(adc_sample > 3000) //clench 
{
    is_clenched = 1; 

} else if(adc_sample < 700)//relase
{
    is_clenched = 0;
    
}



if(is_clenched != present_state){

duty = is_clenched ? max_pwm:min_pwm;

ledc_set_duty_and_update(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,duty,0);
present_state = is_clenched;
}}