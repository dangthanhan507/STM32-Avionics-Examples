#include <nxp_fxas21002c.h>

void fxas21002_interrupt_setup(I2C_HandleTypeDef* hi2c1)
{
	HAL_StatusTypeDef ret[5];
	ret[0] = i2c_transmit(hi2c1, FXAS21002C_ADDRESS, 0x0E, 0x07);
	ret[1] = i2c_transmit(hi2c1, FXAS21002C_ADDRESS, 0x10, 0x40);
	ret[2] = i2c_transmit(hi2c1, FXAS21002C_ADDRESS, 0x14, 0x34);
	if (ret[0] == ret[1] == ret[2] == HAL_OK) printf("Interrupt init went well with FXAS21002C\n");
	/*
	ret[0] = i2c_transmit(hi2c1, FXOS8700_ADDRESS, FXOS8700_REGISTER_CTRL_REG5, 0x01);
	//ret[1] = i2c_transmit(hi2c1, FXOS8700_ADDRESS, 0x0A, 0x18); //modifies TRIG_CFG to have 0x18
	ret[1] = i2c_transmit(hi2c1, FXOS8700_ADDRESS, 0x23, 0x40);
	ret[2] = i2c_transmit(hi2c1, FXOS8700_ADDRESS, 0x24, 0x40);
	ret[3] = i2c_transmit(hi2c1, FXOS8700_ADDRESS, 0x25, 0x40);
	ret[4] = i2c_transmit(hi2c1, FXOS8700_ADDRESS, 0x26, 0xA2);

	ret[5] = i2c_transmit(hi2c1, FXOS8700_ADDRESS, 0x11, 0xC0);
	ret[6] = i2c_transmit(hi2c1, FXOS8700_ADDRESS, 0x14, 0x78);
	if (ret[0] == ret[1] == ret[2] == ret[3] == ret[4] == ret[5] == ret[6] == HAL_OK) printf("Interrupt init went well with FXOS8700\n");
	*/
}

void fxas21002c_init(I2C_HandleTypeDef* hi2c1)
{
	//this is set to 250 dps
	HAL_StatusTypeDef ret[4];
	printf("Setting up FXAS21002C\n");
	ret[0] = i2c_transmit(hi2c1, FXAS21002C_ADDRESS, GYRO_REGISTER_CTRL_REG1, 0x00); //standby
	fxas21002_interrupt_setup(hi2c1);
	ret[1] = i2c_transmit(hi2c1, FXAS21002C_ADDRESS, GYRO_REGISTER_CTRL_REG1, 1 << 6); //reset
	ret[2] = i2c_transmit(hi2c1, FXAS21002C_ADDRESS, GYRO_REGISTER_CTRL_REG0, 0x03); //set sensitivity
	ret[3] = i2c_transmit(hi2c1, FXAS21002C_ADDRESS, GYRO_REGISTER_CTRL_REG1, 0x0E); //active
	HAL_Delay(100);

	if (ret[0] == ret[1] == ret[2] == ret[3] == HAL_OK) printf("Configuration went well with FXAS21002C\n");
	else printf("Something is wrong with FXOS21002C\n");

	printf("Done Setting up FXAS21002C\n");
}

void poll_gyro(I2C_HandleTypeDef* hi2c1, struct GyroscopeData* gyro)
{
	uint8_t buffer[6];
	HAL_StatusTypeDef ret;
	ret = i2c_read(hi2c1, FXAS21002C_ADDRESS, GYRO_REGISTER_STATUS+1, buffer, 6);

	if (ret != HAL_OK) printf("Polling FXOS21002C did not work\n");
	gyro->x = (double)((int16_t)((buffer[1] << 8) | buffer[2])) * GYRO_SENSITIVITY_250DPS;
	gyro->y = (double)((int16_t)((buffer[3] << 8) | buffer[4])) * GYRO_SENSITIVITY_250DPS;
	gyro->z = (double)((int16_t)((buffer[5] << 8) | buffer[6])) * GYRO_SENSITIVITY_250DPS;
}
