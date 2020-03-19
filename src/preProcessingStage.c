/*
*   MIT License
*
*   Copyright (c) 2020 Anna Klingberg Brondin and Marcus Nordström
*/
#include "preProcessingStage.h"
#include <math.h>
#include <stdio.h>
static ring_buffer_t *ppBuf;

void initPreProcessStage(ring_buffer_t *ppBufIn) {
    ppBuf = ppBufIn;
}

void preProcessSample(long time, float x, float y, float z)
{
    double magnitude = sqrt((double)(x*x)+(y*y)+(z*z));
    data_point_t dataPoint;
    dataPoint.time = time;
    dataPoint.magnitude = magnitude;
    ring_buffer_queue(ppBuf, dataPoint);
    printf("preProcess = %f\n", magnitude);
}