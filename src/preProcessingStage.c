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
#include "utils.h"
/*Uncomment this line to disable interpolation*/
//#define STEP_INTERPOLATION_DISABLE
static ring_buffer_t *rawBuf;
static ring_buffer_t *ppBuf;
static int8_t interpolationTime = 10;       //in ms
static int16_t timeScalingFactor = 1; //100000 for validation data or 1000000 for optimisation data (oxford-step-counter dataset)
static int32_t startTime = -1;
static int16_t interpolationCount = 0;

void initPreProcessStage(ring_buffer_t *rawBufIn, ring_buffer_t *ppBufIn)
{
    rawBuf = rawBufIn;
    ppBuf = ppBufIn;
}

static data_point_t linearInterpolate(data_point_t dp1, data_point_t dp2, int64_t interpTime)
{
    long mag = (dp1.magnitude + ((dp2.magnitude - dp1.magnitude) / (dp2.time - dp1.time)) * (interpTime - dp1.time));
    data_point_t interp;
    interp.time = interpTime;
    interp.magnitude = mag;
    return interp;
}

void preProcessSample(int64_t time, int32_t x, int32_t y, int32_t z)
{
    if (startTime == -1) //first point handler
    {
        startTime = time;
    }
    //long magnitude = (labs(x * x) + labs(y * y) + labs(z * z)) / (labs(x) + labs(y) + labs(z));
    int64_t magnitude = isqrt((int64_t)(x*x+y*y+z*z)); //Original
    //long magnitude = (long)sqrt((x*x)+(y*y)+(z*z));
    data_point_t dataPoint;
    dataPoint.time = (time - startTime) / timeScalingFactor;
    dataPoint.magnitude = magnitude;
#ifdef STEP_INTERPOLATION_DISABLE
    ring_buffer_queue(ppBuf, dataPoint);
    filterStage();
#else
    ring_buffer_queue(rawBuf, dataPoint);
    if (ring_buffer_num_items(rawBuf) >= 2)
    {
        data_point_t dp1;
        data_point_t dp2;
        ring_buffer_peek(rawBuf, &dp1, 0);
        ring_buffer_peek(rawBuf, &dp2, 1);
        int16_t numberOfPoints = 1 + ((((dp2.time - dp1.time)) - 1) / interpolationTime); //celing function
        for (int16_t i = 0; i < numberOfPoints; i++)
        {
            int64_t interpTime = (int64_t)interpolationCount * interpolationTime;
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
#endif
}

void resetPreProcess(void)
{
    startTime = -1;
    interpolationCount = 0;
}