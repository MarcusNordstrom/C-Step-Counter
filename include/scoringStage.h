/*
*   MIT License
*
*   Copyright (c) 2020 Anna Klingberg Brondin and Marcus Nordström
*/
#ifndef SCORING_STAGE_H
#define SCORING_STAGE_H
#include "ringbuffer.h"

void initScoringStage(ring_buffer_t *smoothBuf, ring_buffer_t *peakScoreBuf);
void scoringStage(void);

#endif