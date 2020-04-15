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

#include "filterStage.h"
#include "scoringStage.h"
static ring_buffer_t *ppBuf;
static ring_buffer_t *smoothBuf;
static int32_t coeffs[13] = {1687988, 58749795, 16299118, 36044776, 63539094, 8928125, 100000000, 8928125, 63539094, 36044776, 16299118, 58749795, 1687988};
static int32_t filterSum = 525454400;
static int8_t filterLen = 13;

void
initFilterStage(ring_buffer_t * ppBufIn, ring_buffer_t *smoothBufIn)
{
    ppBuf = ppBufIn;
    smoothBuf = smoothBufIn;
}

void filterStage(void)
{
    if (ring_buffer_num_items(ppBuf) == filterLen)
    {
        int64_t sum = 0;
        data_point_t dataPoint;
        for (int8_t i = 0; i < filterLen; i++)
        {
            ring_buffer_peek(ppBuf, &dataPoint, i);
            sum += dataPoint.magnitude * coeffs[i];
        }
        ring_buffer_dequeue(ppBuf, &dataPoint);
        data_point_t out;
        out.time = dataPoint.time;
        out.magnitude = sum / filterSum;
        ring_buffer_queue(smoothBuf, out);
        scoringStage();
    }
}
