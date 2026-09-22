#include "servo.h"
#include "driver/ledc.h"
#define LEDC_GPIO 2 
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
}


void servo_set_pos(bool clinching){
uint8_t min_pwm = 200;
uint8_t max_pwm = 1000;
uint8_t duty = 0;

if(clinching){
    duty = max_pwm;
}
else{
    duty = min_pwm;
}

ledc_set_duty_and_update(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,duty,0);

}