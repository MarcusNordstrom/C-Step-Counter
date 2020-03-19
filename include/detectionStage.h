/*
*   MIT License
*
*   Copyright (c) 2020 Anna Klingberg Brondin and Marcus Nordström
*/
#ifndef DETECTION_STAGE_H
#define DETECTION_STAGE_H
#include "ringbuffer.h"


void initDetectionStage(ring_buffer_t *peakScoreBuf, ring_buffer_t *peakBuf);
void detectionStage(void);

#endif