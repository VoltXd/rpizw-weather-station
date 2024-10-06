#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "SHT3x.h"

#define ONE_MINUTE (60)

void writeHeader(FILE* csvFile, const char* csvFilename);

int main()
{
	int i2cFile;
	const char* i2cFilename = "/dev/i2c-1";

	FILE* csvFile = NULL;
	const char* csvFilename = "/var/www/html/weather-report.csv";

	writeHeader(csvFile, csvFilename);
	
	// Init. I2C
	i2cFile = sht3xInit(i2cFilename);

	// Infinite loop
	while (1)
	{
		// Wait 5 minutes (-1 because of I2C 1 sec delay)
		// Delay first, so that the board has room to update time
		// from internet
		sleep(ONE_MINUTE * 5 - 1);

		// Weather
		float temperature, rh;
		sht3xSingleShot(i2cFile, REP_LOW, &temperature, &rh);

		// Time
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);

		// Print in file "yyyy-mm-dd hh-mm-ss,temp,rh"
		writeHeader(csvFile, csvFilename);
		csvFile = fopen(csvFilename, "a");
		fprintf(csvFile, "%d-%02d-%02d %02d:%02d:%02d,%.1f,%.1f\n", tm.tm_year + 1900,
		                                                            tm.tm_mon + 1,
											                        tm.tm_mday,
											                        tm.tm_hour,
											                        tm.tm_min,
											                        tm.tm_sec,
														            temperature,
														            rh);
		fclose(csvFile);
	}

	// Close I2C (never happens...)
	sht3xDestroy(i2cFile);
	return EXIT_SUCCESS;
}

void writeHeader(FILE *csvFile, const char *csvFilename)
{
	// Print header if needed
	if ((csvFile = fopen(csvFilename, "r")) == NULL)
	{
		csvFile = fopen(csvFilename, "w");
		if (csvFile == NULL)
		{
			printf("Couldn't open %s\n", csvFilename);
			exit(EXIT_FAILURE);
		}
		fprintf(csvFile, "datetime,temp,rh\n");
		fclose(csvFile);
	}
	else
		fclose(csvFile);
}
