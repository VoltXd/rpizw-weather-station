#ifndef SHT3X_H
#define SHT3X_H

#include <stdint.h>

typedef enum
{
	REP_LOW,
	REP_MEDIUM,
	REP_HIGH
} sht3xRepeatability;

int sht3xInit(const char* filename);
void sht3xDestroy(int file);

void sht3xSingleShot(int file, sht3xRepeatability repeatability, float* temperature, float* rh);

#endif
