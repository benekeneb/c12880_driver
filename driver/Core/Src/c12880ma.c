/*
 * c12880ma.c
 *
 *  Created on: Jan 26, 2023
 *      Author: root
 */

#include "c12880ma.h"
#include "main.h"
#include "ads1115.h"

#include <string.h>

enum SensorStates
{
    SensorIdle,          // doing nothing
    SensorStart,         // state to start conversion machine
    SensorStartSampling, // state in which ST signal is high, to start a sensortral sample
    SensorIntegrating,   // state in which sensor is integrating ...
    SensorWaiting,       // state after integration is done, but before reading starts
    SensorReading        // state in which sensor is reading
};

volatile int sensorReady;
volatile enum SensorStates sensorState;
volatile int sensorClockSig;
volatile int sensorIntegrationCycleCountdown;
volatile int sensorTriggerCount;
volatile int sensorDataIndex;

int integrationTimeUS;
int averageCount;
int rawSampleBufferPosition;
uint16_t rawSampleBuffer[SENSOR_MAX_AVERAGE_COUNT][SENSOR_PIXEL_COUNT];

extern TIM_HandleTypeDef htim1;

void setSensorClockMode(char mode){
	switch(mode){
		case 'i': //INTEGRATION MODE --> 250kHz
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_4);
			__HAL_TIM_SET_COUNTER(&htim1, 0);
			htim1.Instance->ARR = 72-1;
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 9);
		break;

		case 'a': //ADC MODE --> 600Hz
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_4);
			__HAL_TIM_SET_COUNTER(&htim1, 0);
			htim1.Instance->ARR = 30000-1;
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 15000);
		break;
	}
}


void sensorInit()
{
    HAL_GPIO_WritePin(Sensor_CLK_GPIO_Port, Sensor_CLK_Pin, 0);
    HAL_GPIO_WritePin(Sensor_ST_GPIO_Port, Sensor_ST_Pin, 0);

    sensorClockSig = 0;
    sensorReady = 0;
    sensorState = SensorIdle;
    sensorDataIndex = 0;

    memset(rawSampleBuffer, 0, sizeof(rawSampleBuffer));
    rawSampleBufferPosition = 0;

    averageCount = SENSOR_MAX_AVERAGE_COUNT;
}

void sensorStartScan(int intTimeUS, int avgCount)
{
    if (sensorState == SensorIdle)
    {
    	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 1);

    	setSensorClockMode('i');

        sensorReady = 0;
        integrationTimeUS = intTimeUS;
        averageCount = avgCount < SENSOR_MAX_AVERAGE_COUNT ? avgCount : SENSOR_MAX_AVERAGE_COUNT;

        sensorState = SensorStart;
        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
    }
}

int sensorIsReady()
{
    return sensorReady;
}

void sensorRead(uint16_t *rawSpectrumBuffer)
{
    double averagedSampleBuffer[SENSOR_PIXEL_COUNT];
    memset(averagedSampleBuffer, 0, sizeof(averagedSampleBuffer));
    int i, j;
    for (i = 0; i < averageCount; i++)
    {
        for (j = 0; j < SENSOR_PIXEL_COUNT; j++)
        {
            averagedSampleBuffer[j] += (double)rawSampleBuffer[i][j];
        }
    }
    for (j = 0; j < SENSOR_PIXEL_COUNT; j++)
    {
        rawSpectrumBuffer[j] = (uint16_t)(averagedSampleBuffer[j] / averageCount);
    }
}

void handleSensorTrigger()
{
    sensorTriggerCount++; // increment the sensorTriggerCount

    switch (sensorState)
    {

    case SensorIntegrating:
        if (sensorTriggerCount >= 48)
        {
            sensorState = SensorWaiting;
        }
        break;

    case SensorStartSampling:
    //    	printf("SensorStartSampling\r\n");
            sensorIntegrationCycleCountdown--;

            if (sensorIntegrationCycleCountdown == 0)
            {
                HAL_GPIO_WritePin(Sensor_ST_GPIO_Port, Sensor_ST_Pin, 0);
                sensorTriggerCount = 0;
                sensorState = SensorIntegrating;
            }

            break;

    case SensorWaiting:
    //    	printf("SensorWaiting\r\n");
            if (sensorTriggerCount >= 87)
            {
            	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_4);
            	__HAL_TIM_SET_COUNTER(&htim1, 0);
                sensorDataIndex = 0;

                setSensorClockMode('a');
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
                sensorState = SensorReading;
            }
            break;

    case SensorStart:
        sensorReady = 0;
        sensorDataIndex = 0;
        sensorIntegrationCycleCountdown = 4 + (integrationTimeUS / (SENSOR_CLK_PERIOD_NS*1000));
        sensorState = SensorStartSampling;
        HAL_GPIO_WritePin(Sensor_ST_GPIO_Port, Sensor_ST_Pin, 1);
        break;




    case SensorReading:
        if (sensorDataIndex < 288)
        {
        	uint16_t adc_val = adcRead();
            rawSampleBuffer[rawSampleBufferPosition][sensorDataIndex] = adc_val;
        }

        sensorDataIndex++;

        if (sensorDataIndex >= 288)
        {


            rawSampleBufferPosition++;
            if (rawSampleBufferPosition > averageCount)
            {
                rawSampleBufferPosition = 0;
                sensorReady = 1;
                sensorState = SensorIdle;
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_4);
                __HAL_TIM_SET_COUNTER(&htim1, 0);
                HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 0);
                setSensorClockMode('i');
            }
            else
            {
                sensorState = SensorStart;
                setSensorClockMode('i');
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
            }
        }
        break;

    default:
        break;
    }
}

int sensorGetIntegrationTime()
{
    return integrationTimeUS;
}

int sensorGetAverageCount()
{
    return averageCount;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == Sensor_TRG_Pin)
    {
        handleSensorTrigger();
    }
}




