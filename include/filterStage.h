/*
*   MIT License
*
*   Copyright (c) 2020 Anna Klingberg Brondin and Marcus Nordström
*/
#ifndef FILTER_STAGE_H
#define FILTER_STAGE_H
#include "ringbuffer.h"


void initFilterStage(ring_buffer_t *ppBuf, ring_buffer_t *smoothBuf);
void filterStage(void);

#endif