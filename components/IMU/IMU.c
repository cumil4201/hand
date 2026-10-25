#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include <stdio.h>   

#define INTERUPT_GPIO 4
#define SDA_PIN 21
#define SCL_PIN 22


i2c_master_dev_handle_t IMU_handle;
i2c_master_bus_handle_t bus_handle;



void imu_init(void){

//GPIO intr config
gpio_config_t gpio_conf = {
    .intr_type = GPIO_INTR_POSEDGE,
    .mode = GPIO_MODE_INPUT,
    .pin_bit_mask = (1ULL<<INTERUPT_GPIO),
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .pull_up_en = GPIO_PULLUP_DISABLE
    
};

gpio_config(&gpio_conf);
//i2c config 



i2c_master_bus_config_t i2c_config = {
.clk_source = I2C_CLK_SRC_DEFAULT,
.i2c_port = I2C_NUM_0,
.scl_io_num = SCL_PIN,
.sda_io_num = SDA_PIN,
.glitch_ignore_cnt = 7,
.flags.enable_internal_pullup = true

};

ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_config,&bus_handle));



i2c_device_config_t dev_config ={
    .dev_addr_length = I2C_ADDR_BIT_LEN_7,
    .device_address = 0x68,
    .scl_speed_hz= 400000

};


ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle,&dev_config,&IMU_handle));

uint8_t wake_data[2] = {0x6B, 0x00}; 
i2c_master_transmit(IMU_handle, wake_data, 2, -1);



}



void readIMU(){
uint8_t IMU_reg = 0x3B;
uint8_t rx_data[6];


esp_err_t err = i2c_master_transmit_receive(IMU_handle,&IMU_reg,1,rx_data,6,-1);

if(ESP_OK == err){
// Sklejanie dwóch 8-bitowych rejestrów w jedną 16-bitową liczbę całkowitą ze znakiem
        int16_t accel_x = (rx_data[0] << 8) | rx_data[1];
        int16_t accel_y = (rx_data[2] << 8) | rx_data[3];
        int16_t accel_z = (rx_data[4] << 8) | rx_data[5];

        printf("Akcelerometr -> X: %d, Y: %d, Z: %d\n", accel_x, accel_y, accel_z);



}


}






