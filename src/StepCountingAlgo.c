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
#include "StepCountingAlgo.h"
#include "ringbuffer.h"
#include "preProcessingStage.h"
#include "filterStage.h"
#include "scoringStage.h"
#include "detectionStage.h"
#include "postProcessingStage.h"
// General data
static int steps;
static int sampleFreq;
// Buffers
static ring_buffer_t rawBuf;
static ring_buffer_t ppBuf;
static ring_buffer_t smoothBuf;
static ring_buffer_t peakScoreBuf;
static ring_buffer_t peakBuf;

void initAlgo(int sampleFreqIn)
{
    sampleFreq = sampleFreqIn;
    ring_buffer_init(&rawBuf);
    ring_buffer_init(&ppBuf);
    initPreProcessStage(&rawBuf, &ppBuf);
    ring_buffer_init(&smoothBuf);
    initFilterStage(&ppBuf, &smoothBuf);
    ring_buffer_init(&peakScoreBuf);
    initScoringStage(&smoothBuf, &peakScoreBuf);
    ring_buffer_init(&peakBuf);
    initDetectionStage(&peakScoreBuf, &peakBuf);
    initPostProcessingStage(&peakBuf, &steps);
}

void processSample(long time, float x, float y, float z)
{
    preProcessSample(time, x, y, z);
}

void resetSteps(void)
{
    steps = 0;
}

int getSteps(void)
{
    return steps;
}

int getSampleFreq()
{
    return sampleFreq;
}
