/*
*   MIT License
*
*   Copyright (c) 2020 Anna Klingberg Brondin and Marcus Nordström
*/
#include "detectionStage.h"
#include <math.h>
#include <stdio.h>
static ring_buffer_t *peakScoreBuf;
static ring_buffer_t *peakBuf;
static float mean = 0;
static float std = 0;
static int count = 0; //Might overflow, needs revision
static float threshold = 1.2;

void initDetectionStage(ring_buffer_t *peakScoreBufIn, ring_buffer_t *peakBufIn)
{
    peakScoreBuf = peakScoreBufIn;
    peakBuf = peakBufIn;
}

void detectionStage(void)
{
    if (!ring_buffer_is_empty(peakScoreBuf))
    {
        float oMean = mean;
        data_point_t dataPoint;
        ring_buffer_dequeue(peakScoreBuf, &dataPoint);
        count++;
        switch (count)
        {
        case 1:
            mean = dataPoint.magnitude;
            std = 0;
            break;
        case 2:
            mean = (mean + dataPoint.magnitude) / 2;
            std = (float)sqrt(pow(dataPoint.magnitude - mean, 2) + pow(oMean - mean, 2)) / 2;
            break;
        default:
            mean = (dataPoint.magnitude + ((count - 1) * mean)) / count;
            std = (float)sqrt(((count - 2) * pow(std, 2) / (count - 1)) + pow(oMean - mean, 2) + pow(dataPoint.magnitude - mean, 2) / count);
            break;
        }
        if (count > 15)
        {
            if ((dataPoint.magnitude - mean) > std * threshold)
            {
                // This is a peak
                ring_buffer_queue(peakBuf, dataPoint);
                printf("detection = %f\n", dataPoint.magnitude);
            }
        }
    }
}