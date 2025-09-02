/*
 * main.c:
 * Copyright (c) 2014-2024 Rtrobot. <admin@rtrobot.org>
 *  <http://rtrobot.org>
 ***********************************************************************
 */

#include <stdio.h>
#include <nvs_flash.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include <driver/i2c.h>
#include "vl53lx_test.h"

#define I2C_MASTER_SDA_IO (gpio_num_t)18
#define I2C_MASTER_SCL_IO (gpio_num_t)19
#define I2C_MASTER_FREQ_HZ 400000

/***************************************************************************************************************
i2c master initialization
****************************************************************************************************************/
esp_err_t rtrobot_i2c_init(void)
{
	i2c_config_t conf;
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = I2C_MASTER_SDA_IO;
	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
	conf.scl_io_num = I2C_MASTER_SCL_IO;
	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
	conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
	i2c_param_config(I2C_NUM_0, &conf);
	return i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);
}

void i2c_VL53L4CD_task(void *pvParameters)
{
	rtrobot_i2c_init();
	vl53lx_init();
	while (1)
	{
		vl53lx_test(13);
	}
}

void app_main()
{
	nvs_flash_init();
	xTaskCreate(i2c_VL53L4CD_task, "i2c_VL53L4CD_task", 8192*8, NULL, 1, NULL);
}
