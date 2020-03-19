/*
*   MIT License
*
*   Copyright (c) 2020 Anna Klingberg Brondin and Marcus Nordström
*/
#include "postProcessingStage.h"
#include <stdio.h>
static ring_buffer_t *peakBuf;
static int *steps;
static data_point_t lastDataPoint = {0, 0};
static int timeThreshold = 200;
void initPostProcessingStage(ring_buffer_t *peakBufIn, int *stepsIn)
{
    peakBuf = peakBufIn;
    steps = stepsIn;
}

void postProcessingStage(void)
{
    if (!ring_buffer_is_empty(peakBuf))
    {
        data_point_t dataPoint;
        ring_buffer_dequeue(peakBuf, &dataPoint);
        if (lastDataPoint.time == 0)
        {
            lastDataPoint = dataPoint;
        }
        else
        {
            if ((dataPoint.time - lastDataPoint.time) > timeThreshold)
            {
                lastDataPoint = dataPoint;
                steps++;
            }
            else
            {
                if (dataPoint.magnitude > lastDataPoint.magnitude)
                {
                    lastDataPoint = dataPoint;
                }
            }
        }
    }
}