/*
*   MIT License
*
*   Copyright (c) 2020 Anna Klingberg Brondin and Marcus Nordström
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
static ring_buffer_t ppBuf;
static ring_buffer_t smoothBuf;
static ring_buffer_t peakScoreBuf;
static ring_buffer_t peakBuf;

void initAlgo(int sampleFreqIn)
{
    sampleFreq = sampleFreqIn;
    ring_buffer_init(&ppBuf);
    initPreProcessStage(&ppBuf);
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
    filterStage();
    scoringStage();
    detectionStage();
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
