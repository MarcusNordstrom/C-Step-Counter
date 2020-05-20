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
#include "scoringStage.h"
#include "detectionStage.h"
static ring_buffer_t *smoothBuf;
static ring_buffer_t *peakScoreBuf;
static ring_buffer_size_t windowSize = 40;
static ring_buffer_size_t midpoint = 20; //half of size

void initScoringStage(ring_buffer_t *smoothBufIn, ring_buffer_t *peakScoreBufIn)
{
    smoothBuf = smoothBufIn;
    peakScoreBuf = peakScoreBufIn;
}

void scoringStage(void)
{
    if (ring_buffer_num_items(smoothBuf) == windowSize)
    {
        int64_t diffLeft = 0;
        int64_t diffRight = 0;
        data_point_t midpointData;
        ring_buffer_peek(smoothBuf, &midpointData, midpoint);
        data_point_t dataPoint;
        for (ring_buffer_size_t i = 0; i < midpoint; i++)
        {
            ring_buffer_peek(smoothBuf, &dataPoint, i);
            diffLeft += midpointData.magnitude - dataPoint.magnitude;
        }
        for (ring_buffer_size_t j = midpoint + 1; j < windowSize; j++)
        {
            ring_buffer_peek(smoothBuf, &dataPoint, j);
            diffRight += midpointData.magnitude - dataPoint.magnitude;
        }
        long scorePeak = (diffLeft + diffRight) / (windowSize - 1);
        data_point_t out;
        out.time = midpointData.time;
        out.magnitude = scorePeak;
        ring_buffer_queue(peakScoreBuf, out);
        ring_buffer_dequeue(smoothBuf, &midpointData);
        detectionStage();
    }
}

void changeWindowSize(ring_buffer_size_t windowsize)
{
    windowSize = windowsize;
    midpoint = windowsize / 2;
}