#pragma once 
#include <stdint.h>



void servo_init();
void servo_set_pos(uint32_t adc_sample);