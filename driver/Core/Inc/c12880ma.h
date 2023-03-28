/*
 * c12880ma.h
 *
 *  Created on: Jan 26, 2023
 *      Author: root
 */

#ifndef INC_C12880MA_H_
#define INC_C12880MA_H_

#include "stm32f3xx_hal.h"

#define SENSOR_PIXEL_COUNT 288
#define SENSOR_MAX_AVERAGE_COUNT 32
#define SENSOR_CLK_PERIOD_NS 400

#define PROTOCOL_START 0xaabb
#define PROTOCOL_DATA_START 0xaf0f
#define PROTOCOL_END 0xcccc
#define PROTOCOL_PAD_SIZE (1024 - (10 + (SENSOR_PIXEL_COUNT * 2)))

typedef struct __attribute__((__packed__))
{
    uint16_t start;
    uint16_t integrationTime;
    uint16_t averageCount;
    uint16_t end;
} ProtocolCommandFrame;

typedef struct __attribute__((__packed__))
{
    uint16_t start;
    uint16_t integrationTime;
    uint16_t averageCount;
    uint16_t dataStart;
    uint16_t spectrum[SENSOR_PIXEL_COUNT];
    uint16_t end;
    uint8_t pad[PROTOCOL_PAD_SIZE];
} ProtocolResponseFrame;

void sensorInit();
void setSensorClockMode(char mode);
void sensorStartScan(int intTimeMS, int avgCount);
int sensorIsReady();
void sensorRead(uint16_t *rawSpectrumBuffer);
int sensorGetIntegrationTime();
int sensorGetAverageCount();


#endif /* INC_C12880MA_H_ */
