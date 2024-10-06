#include "SHT3x.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>

#define SHT3X_I2C_ADDR (0x44)
#define STH3X_SS_CLD_REG (0x24)
#define STH3X_SS_CLD_LOW (0x16)
#define STH3X_SS_CLD_MEDIUM (0x0B)
#define STH3X_SS_CLD_HIGH (0x00)

uint8_t isInitialised = 0;

int sht3xInit(const char* filename)
{
	// Open I2C device file
	int file = open(filename, O_RDWR);
	if (file < 0)
	{
		printf("Error: open device failure.\n");
		printf("\tterminating program...\n");
		close(file);
		exit(EXIT_FAILURE);
	}

	// Specify I2C device address
	if (ioctl(file, I2C_SLAVE, SHT3X_I2C_ADDR) < 0)
	{
		printf("Error: I2C address specification failure.\n");
		printf("\tterminating program...\n");
		close(file);
		exit(EXIT_FAILURE);
	}

	isInitialised = 1;

	return file;
}

void sht3xDestroy(int file)
{
	close(file);
	isInitialised = 0;
}

static float sht3xCalculateTemperature(uint8_t msb, uint8_t lsb)
{
	// Calculate integer coded value
	uint16_t temperatureUint = ((uint16_t)msb << 8) | lsb; 
	
	// Convert from uint [0, 2^16 - 1] to floats [-45, 130]
	float temperature = -45.0f + temperatureUint * (175.0f / 0xFFFF);
	return temperature;
}

static float sht3xCalculateRH(uint8_t msb, uint8_t lsb)
{
	// Calculate integer coded value
	uint16_t rhUint = ((uint16_t)msb << 8) | lsb; 

	// Convert from uint [0, 2^16 - 1] to floats [0, 100]
	float rh = rhUint * (100.0f / 0xFFFF);
	return rh;
}

void sht3xSingleShot(int file, sht3xRepeatability repeatability, float *temperature, float *rh)
{
	// Assert I2C initialised
	if (!isInitialised)
	{
		printf("Error: I2C not initialised.\n");
		printf("\tterminating program...\n");
		exit(EXIT_FAILURE);

	}

	// Assert no nullptr
	if (temperature == NULL ||
	    rh == NULL)
	{
		printf("Error: I2C write failure.\n");
		printf("\tterminating program...\n");
		close(file);
		exit(EXIT_FAILURE);
	}

	uint8_t buffer[6] = { 0 };

	// Single shot measurement
	// Order measurement
	buffer[0] = STH3X_SS_CLD_REG;
	if (repeatability == REP_LOW)
		buffer[1] = STH3X_SS_CLD_LOW;
	else if (repeatability == REP_MEDIUM)
		buffer[1] = STH3X_SS_CLD_MEDIUM;
	else if (repeatability == REP_HIGH)
		buffer[1] = STH3X_SS_CLD_HIGH;

	if (write(file, buffer, 2) != 2)
	{
		printf("Error: I2C write failure.\n");
		printf("\tterminating program...\n");
		close(file);
		exit(EXIT_FAILURE);
	}

	sleep(1);
	
	// Retrieve measurement
	int numBytesReceived;
	if ((numBytesReceived = read(file, buffer, 6)) != 6) 
	{
		printf("Error: I2C read (%d bytes) failure.\n", numBytesReceived);
		printf("\tterminating program...\n");
		close(file);
		exit(EXIT_FAILURE);
  	}

#ifdef DEBUG_MODE
	int i;
	for (i = 0; i < 6; i++)
		printf("%d ", buffer[i]);
	printf("\n");
#endif

	*temperature = sht3xCalculateTemperature(buffer[0], buffer[1]);
	*rh = sht3xCalculateRH(buffer[3], buffer[4]);
}
