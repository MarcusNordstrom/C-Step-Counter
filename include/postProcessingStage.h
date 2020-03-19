/*
*   MIT License
*
*   Copyright (c) 2020 Anna Klingberg Brondin and Marcus Nordström
*/
#ifndef POST_PROCESSING_STAGE_H
#define POST_PROCESSING_STAGE_H
#include "ringbuffer.h"


void initPostProcessingStage(ring_buffer_t *peakBuf, int *steps);
void postProcessingStage(void);

#endif