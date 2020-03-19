/*
*   MIT License
*
*   Copyright (c) 2020 Anna Klingberg Brondin and Marcus Nordström
*/
#include "filterStage.h"
#include <stdio.h>
static ring_buffer_t *ppBuf;
static ring_buffer_t *smoothBuf;
static float coeffs[13] = {0.01687988, 0.058749795, 0.16299118, 0.36044776, 0.63539094, 0.8928125, 1.0, 0.8928125, 0.63539094, 0.36044776, 0.16299118, 0.058749795, 0.01687988};
static float filterSum = 5.254544;
static int filterLen = 13;

void initFilterStage(ring_buffer_t *ppBufIn, ring_buffer_t *smoothBufIn)
{
    ppBuf = ppBufIn;
    smoothBuf = smoothBufIn;
}

void filterStage(void)
{
    if (ring_buffer_num_items(ppBuf) == filterLen)
    {
        float sum = 0;
        data_point_t dataPoint;
        for (int i = 0; i < filterLen; i++)
        {
            ring_buffer_peek(ppBuf, &dataPoint, i);
            sum += dataPoint.magnitude * coeffs[i];
        }
        ring_buffer_dequeue(ppBuf, &dataPoint);
        data_point_t out = {dataPoint.time, (sum/filterSum)};
        ring_buffer_queue(smoothBuf, out);
        printf("filter = %f\n", sum / filterSum);
    }
}
