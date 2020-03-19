/*
*   MIT License
*
*   Copyright (c) 2020 Anna Klingberg Brondin and Marcus Nordström
*/
#ifndef STEP_COUNTING_ALGO_H
#define STEP_COUNTING_ALGO_H

void initAlgo(int sampleFreq);

/*
    This function takes the raw accelerometry data and computes the entire algorithm
    @param time, the current time in ms
    @param x, the x axis
    @param y, the y axis
    @param z, the z axis
*/
void processSample(long time, float x, float y, float z);

/*
    Sets the filter coefficiants for the FIR-filter, these can be calculated using the script
    @param coeffs, a memory allocated array of coefficiants, DO NOT FREE MANUALLY
    @param len, the length of the array.
*/
void setFilterCoeffs(float *coeffs, int len);

/*
    Resets the number of walked steps
*/
void resetSteps(void);

/*
    Returns the number of walked steps
    @return steps walked
*/
int getSteps(void);

/*
    Retuns the current sample frequency
    @return sample frequency    
*/
int getSampleFreq(void);

#endif