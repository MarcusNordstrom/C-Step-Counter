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
#include <stdio.h>
static ring_buffer_t *ppBuf;
static ring_buffer_t *smoothBuf;
static float coeffs[13] = {0.01687988, 0.058749795, 0.16299118, 0.36044776, 0.63539094, 0.8928125, 1.0, 0.8928125, 0.63539094, 0.36044776, 0.16299118, 0.058749795, 0.01687988};
static float filterSum = 5.254544;
static int filterLen = 13;
/*static float coeffs[20] = {0.03035425209, 0.03562249988, 0.040788766, 0.04571138322, 0.05024948716,
                           0.05426871777, 0.05764679983, 0.06027878076, 0.06208159775, 0.06299771369,
                           0.06299771369, 0.06208159775, 0.06027878076, 0.05764679983, 0.05426871777,
                           0.05024948716, 0.04571138322, 0.040788766, 0.03562249988, 0.03035425209};
static int filterLen = 20;*/

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
        float sum = 0;
        data_point_t dataPoint;
        for (int i = 0; i < filterLen; i++)
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
