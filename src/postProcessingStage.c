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

#include "postProcessingStage.h"
static ring_buffer_t *peakBuf;
static data_point_t lastDataPoint;
static int16_t timeThreshold = 200;
static void (*stepCallback)(void);
void initPostProcessingStage(ring_buffer_t *peakBufIn, void (*stepCallbackIn)(void))
{
    peakBuf = peakBufIn;
    stepCallback = stepCallbackIn;
    lastDataPoint.time = 0;
    lastDataPoint.magnitude = 0;
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
                (*stepCallback)();
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

void resetPostProcess(void)
{
    lastDataPoint.magnitude = 0;
    lastDataPoint.time = 0;
}

void changeTimeThreshold(int16_t thresh)
{
    timeThreshold = thresh;
}