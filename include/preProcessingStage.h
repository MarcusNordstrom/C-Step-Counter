/*
*   MIT License
*
*   Copyright (c) 2020 Anna Klingberg Brondin and Marcus Nordström
*/
#ifndef PRE_PROCESSING_STAGE_H
#define PRE_PROCESSING_STAGE_H
#include "ringbuffer.h"

void initPreProcessStage(ring_buffer_t *ppBuf);
void preProcessSample(long time, float x, float y, float z);

#endif