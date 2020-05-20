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

#include "detectionStage.h"
#include "postProcessingStage.h"
#include "utils.h"
//#include <math.h>
static ring_buffer_t *peakScoreBuf;
static ring_buffer_t *peakBuf;
static int64_t mean = 0;
static int64_t std = 0;
static int64_t count = 0;
static int16_t threshold_int = 2;
static int16_t threshold_frac = 2;

void initDetectionStage(ring_buffer_t *peakScoreBufIn, ring_buffer_t *peakBufIn)
{
    peakScoreBuf = peakScoreBufIn;
    peakBuf = peakBufIn;
}

void detectionStage(void)
{
    if (!ring_buffer_is_empty(peakScoreBuf))
    {
        long oMean = mean;
        data_point_t dataPoint;
        ring_buffer_dequeue(peakScoreBuf, &dataPoint);
        count++;
        if (count == 1)
        {
            mean = dataPoint.magnitude;
            std = 0;
        }
        else if (count == 2)
        {
            mean = (mean + dataPoint.magnitude) / 2;
            std = isqrt(((dataPoint.magnitude - mean) * (dataPoint.magnitude - mean)) + ((oMean - mean) * (oMean - mean))) / 2;
            //std = (long)sqrt(pow(dataPoint.magnitude - mean, 2) + pow(oMean - mean, 2)) / 2;
        }
        else
        {
            mean = (dataPoint.magnitude + ((count - 1) * mean)) / count;
            //Split into parts to avoid overflow
            int64_t part1 = ((std*std) / (count-1)) * (count-2);
            int64_t part2 = ((oMean - mean) * (oMean - mean));
            int64_t part3 = ((dataPoint.magnitude - mean) * (dataPoint.magnitude - mean)) / count;
            std = isqrt(part1 + part2 + part3);
            //std = (long)sqrt(((count - 2) * pow(std, 2) / (count - 1)) + pow(oMean - mean, 2) + pow(dataPoint.magnitude - mean, 2) / count);
        }
        if (count > 15)
        {
            if ((dataPoint.magnitude - mean) > (std*threshold_int + (std/threshold_frac)))
            {
                // This is a peak
                ring_buffer_queue(peakBuf, dataPoint);
                postProcessingStage();
            }
        }
    }
}

void resetDetection(void)
{
    std = 0;
    mean = 0;
    count = 0;
}

void changeDetectionThreshold(int16_t whole, int16_t frac)
{
    threshold_int = whole;
    threshold_frac = frac;
}