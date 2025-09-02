/*********************************************************
 * platform.c
 * Copyright (c) 2012 - 2024 RTrobot Inc.
 *
 * Unless otherwise stated, the use of this software is subject to the following conditions:
 * 1. Any form of redistribution must include the original copyright notice and the following disclaimer.
 * 2. Not for commercial use without explicit written permission, including but not limited to sales, licensing, or commercial support.
 * 3. Any modifications to this software must be clearly marked with attribution and documented in the modified files.
 * 4. If modifications are made, they must be clearly indicated in the modified files.
 * 5. Without explicit written permission, the names of the authors or original contributors may not be used to endorse or promote derived products.
 *
 * This software is provided "as is," without any warranties of any kind, express or implied, including but not limited to the warranties of merchantability or fitness for a particular purpose.
 * The authors are not liable for any direct, indirect, incidental, special, exemplary, or consequential damages arising in any way out of the use of this software.
 *********************************************************/


#include "platform.h"
#include <driver/i2c.h>

uint8_t RdByte(VL53LMZ_Platform* p_platform, uint16_t RegisterAdress, uint8_t* p_value)
{
	uint8_t data_write[2];
	data_write[0] = (RegisterAdress >> 8) & 0xFF;
	data_write[1] = RegisterAdress & 0xFF;

	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (p_platform->address) | I2C_MASTER_WRITE, true);
	i2c_master_write(cmd, data_write, 2, true);
	vTaskDelay(15 / portTICK_PERIOD_MS);
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (p_platform->address) | I2C_MASTER_READ, true);
	i2c_master_read_byte(cmd, p_value, (i2c_ack_type_t)0x01);
	i2c_master_stop(cmd);
	esp_err_t status = i2c_master_cmd_begin(I2C_NUM_0, cmd, 200 / portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);
	return status;
}

uint8_t WrByte(VL53LMZ_Platform* p_platform, uint16_t RegisterAdress, uint8_t value)
{
	uint8_t data_write[3];
	data_write[0] = (RegisterAdress >> 8) & 0xFF;
	data_write[1] = RegisterAdress & 0xFF;
	data_write[2] = value & 0xFF;

	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (p_platform->address) | I2C_MASTER_WRITE, true);
	i2c_master_write(cmd, data_write, 3, true);

	i2c_master_stop(cmd);
	esp_err_t status = i2c_master_cmd_begin(I2C_NUM_0, cmd, 200 / portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);
	return status;
}

uint8_t WrMulti(VL53LMZ_Platform* p_platform, uint16_t RegisterAdress, uint8_t* p_values, uint32_t size)
{
	uint8_t *buf = malloc(size + 2);
	buf[0] = (RegisterAdress >> 8) & 0xFF;
	buf[1] = RegisterAdress & 0xFF;
	memcpy(buf + 2, p_values, size);
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (p_platform->address) | I2C_MASTER_WRITE, true);
	i2c_master_write(cmd, buf, size+2, true);
	i2c_master_stop(cmd);
	esp_err_t status = i2c_master_cmd_begin(I2C_NUM_0, cmd, 100 / portTICK_PERIOD_MS);
	free(buf);
	return status;
}

uint8_t RdMulti(VL53LMZ_Platform* p_platform, uint16_t RegisterAdress, uint8_t* p_values, uint32_t size)
{
	uint8_t data_write[2];
	data_write[0] = (RegisterAdress >> 8) & 0xFF;
	data_write[1] = RegisterAdress & 0xFF;

	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (p_platform->address) | I2C_MASTER_WRITE, true);
	i2c_master_write(cmd, data_write, 2, true);
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (p_platform->address) | I2C_MASTER_READ, true);
	if (size > 1)
		i2c_master_read(cmd, p_values, size - 1, (i2c_ack_type_t)0x0);
	i2c_master_read_byte(cmd, p_values + size - 1, (i2c_ack_type_t)0x01);
	i2c_master_stop(cmd);
	esp_err_t status = i2c_master_cmd_begin(I2C_NUM_0, cmd, 65535 / portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);
	return status;
}

void SwapBuffer(uint8_t* buffer, uint16_t size)
{
	uint32_t i, tmp;
	for (i = 0; i < size; i = i + 4)
	{
		tmp = (buffer[i] << 24) | (buffer[i + 1] << 16) | (buffer[i + 2] << 8) | (buffer[i + 3]);

		memcpy(&(buffer[i]), &tmp, 4);
	}
}

uint8_t WaitMs(VL53LMZ_Platform* p_platform, uint32_t TimeMs)
{
	vTaskDelay(TimeMs / portTICK_PERIOD_MS);
	return 0;
}
