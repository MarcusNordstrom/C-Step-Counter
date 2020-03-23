/* 
The MIT License (MIT)

Copyright (c) 2020 Anna Brondin and Marcus Nordström

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "preProcessingStage.h"
#include "filterStage.h"
#include <math.h>
#include <stdio.h>
static ring_buffer_t *rawBuf;
static ring_buffer_t *ppBuf;
static int interpolationTime = 10; //in ms
static float timeScalingFactor = 1000000; //(100000 for validation data)
static float startTime = -1;
static int interpolationCount = 0;

void initPreProcessStage(ring_buffer_t *rawBufIn, ring_buffer_t *ppBufIn)
{
    rawBuf = rawBufIn;
    ppBuf = ppBufIn;
}

static data_point_t linearInterpolate(data_point_t dp1, data_point_t dp2, long interpTime)
{
    //float dt = (float) dp2.time - dp1.time;
    //float dv = dp2.magnitude - dp1.magnitude;
    //float mag = (dv/dt) * ((float)interpTime - dp1.time) + dp1.magnitude;
    long mag = (long) (dp1.magnitude + ((dp2.magnitude - dp1.magnitude) / ((float)(dp2.time - dp1.time))) * (interpTime - dp1.time));
    data_point_t interp;
    interp.time = interpTime;
    interp.magnitude = mag;
    return interp;
}

void preProcessSample(long time, long x, long y, long z)
{
    if (startTime == -1) //first point handler
    {
        startTime = time;
    }
    long magnitude = (long)sqrt((double)(x * x) + (y * y) + (z * z));
    data_point_t dataPoint;
    dataPoint.time = (time-startTime)/timeScalingFactor;
    dataPoint.magnitude = magnitude;
    ring_buffer_queue(rawBuf, dataPoint);
    if (ring_buffer_num_items(rawBuf) >= 2)
    {
        data_point_t dp1;
        data_point_t dp2;
        ring_buffer_peek(rawBuf, &dp1, 0);
        ring_buffer_peek(rawBuf, &dp2, 1);
        int numberOfPoints = (int)ceil((double)(dp2.time - dp1.time) / interpolationTime);
        for (int i = 0; i < numberOfPoints; i++)
        {
            //long interpTime = (long)dp1.time + i * ((dp2.time - dp1.time) / numberOfPoints);
            long interpTime = (long) interpolationCount * interpolationTime;
            if (dp1.time <= interpTime && interpTime < dp2.time)
            {
                data_point_t interpolated = linearInterpolate(dp1, dp2, interpTime);
                ring_buffer_queue(ppBuf, interpolated);
                filterStage();
                interpolationCount++;
            }
        }
        data_point_t dataPoint;
        ring_buffer_dequeue(rawBuf, &dataPoint);
    }

    //printf("preProcess = %f\n", magnitude);
}