#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include "orbit_timing.h"
#include "memtest.h"
#include "orbit_cpu_affinity.h"

size_t count1Bits(char value)
{
    size_t numOnes = 0;
    for(size_t i = 0; i < CHAR_BIT * sizeof value; ++i, value >>= 1)
    {
        if ((value & 1) == 1)
        {
            ++numOnes;
        }
    }
    return numOnes;
}

size_t count0Bits(char value)
{
    return (CHAR_BIT * sizeof value) - count1Bits(value);
}

void cacheEff_setZeros(char *matrix, const size_t dim)
{
    size_t i, j;

    for (i = 0; i < dim * dim; i++)
    {
        matrix[i] = 0;
    }
}

size_t cacheEff_checkZerosAndFlip(char *matrix, const size_t dim)
{
    size_t i, j;
    size_t zeroToOneFlips = 0;

    for (i = 0; i < dim * dim; i++)
    {
        if(matrix[i] != 0)
        {
            zeroToOneFlips += count1Bits(matrix[i]);
        }
        matrix[i] = 1;
    }

    return zeroToOneFlips;
}

size_t cacheEff_checkOnesAndFlip(char *matrix, const size_t dim)
{
    size_t i, j;
    size_t oneToZeroFlips = 0;

    for (i = 0; i < dim * dim; i++)
    {
        if(matrix[i] != 1)
        {
            oneToZeroFlips += count0Bits(matrix[i]);
        }
        matrix[i] = 0;
    }

    return oneToZeroFlips;
}

void cacheIneff_setZeros(char *matrix, const size_t dim)
{
    size_t i, j;

    for (i = 0; i < dim; i++)
    {
        for (j = 0; j < dim; j++)
        {
            matrix[j * dim + i] = 0;
        }
    }
}

size_t cacheIneff_checkZerosAndFlip(char *matrix, const size_t dim)
{
    size_t i, j;
    size_t zeroToOneFlips = 0;

    for (i = 0; i < dim; i++)
    {
        for (j = 0; j < dim; j++)
        {
            if(matrix[j * dim + i] != 0)
            {
                zeroToOneFlips += count1Bits(matrix[j * dim + i]);
            }
            matrix[j * dim + i] = 1;
        }
    }

    return zeroToOneFlips;
}

size_t cacheIneff_checkOnesAndFlip(char *matrix, const size_t dim)
{
    size_t i, j;
    size_t zeroToOneFlips = 0;

    for (i = 0; i < dim; i++)
    {
        for (j = 0; j < dim; j++)
        {
            if(matrix[j * dim + i] != 1)
            {
                zeroToOneFlips += count0Bits(matrix[j * dim + i]);
            }
            matrix[j * dim + i] = 0;
        }
    }

    return zeroToOneFlips;
}

/*char** allocateMatrix(const size_t dim)
{
    char** matrix = new char *[dim];
    for (size_t i = 0; i < dim; ++i)
    {
        matrix[i] = new char[dim];
    }
    return matrix;
}

void deallocateMatrix(char** matrix, size_t dim)
{
    for (size_t a = 0; a < dim; ++a)
    {
        delete [] matrix[a];
    }
    delete [] matrix;
}*/

void memtest_cacheEff_core(char *matrix, size_t dim, useconds_t sleepTime, const char *logTag)
{
    long double millisecs;
    size_t tmpResult;
    memtest_results totalResults = {0,0};

    timestamp_t t0 = get_timestamp();

    //------------------------------------------------------

    // Set 0's
    cacheEff_setZeros(matrix, dim);

    // Let radiation do its thing
    usleep(sleepTime);

    // Check 0's, Set 1's
    tmpResult = cacheEff_checkZerosAndFlip(matrix, dim);
    totalResults.zeroToOneFlips += tmpResult;

    // Let radiation do its thing
    usleep(sleepTime);

    // Check 1's, Set 0's
    tmpResult = cacheEff_checkOnesAndFlip(matrix, dim);
    totalResults.oneToZeroFlips += tmpResult;

    // Let radiation do its thing
    usleep(sleepTime);

    //------------------------------------------------------

    timestamp_t t1 = get_timestamp();

    millisecs = (t1 - t0) / 1000.0L;

    printf("id = %s, 0To1 = %ld, 1To0 = %ld, time = %Lf\n", logTag, totalResults.zeroToOneFlips, totalResults.oneToZeroFlips, millisecs);
}

void memtest_cacheIneff_core(char *matrix, size_t dim, useconds_t sleepTime, const char *logTag)
{
    long double millisecs;
    size_t tmpResult;
    memtest_results totalResults = {0,0};

    timestamp_t t0 = get_timestamp();

    //------------------------------------------------------

    // Set 0's
    cacheIneff_setZeros(matrix, dim);

    // Let radiation do its thing
    usleep(sleepTime);

    // Check 0's, Set 1's
    tmpResult = cacheIneff_checkZerosAndFlip(matrix, dim);
    totalResults.zeroToOneFlips += tmpResult;

    // Let radiation do its thing
    usleep(sleepTime);

    // Check 1's, Set 0's
    tmpResult = cacheIneff_checkOnesAndFlip(matrix, dim);
    totalResults.oneToZeroFlips += tmpResult;

    // Let radiation do its thing
    usleep(sleepTime);

    //------------------------------------------------------

    timestamp_t t1 = get_timestamp();

    millisecs = (t1 - t0) / 1000.0L;

    printf("id = %s, 0To1 = %ld, 1To0 = %ld, time = %Lf\n", logTag, totalResults.zeroToOneFlips, totalResults.oneToZeroFlips, millisecs);
}

void memtest_cacheEff(useconds_t sleepTime, size_t dim, size_t num, const char *logTag)
{
    char *matrix = reinterpret_cast<char*>(malloc(dim * dim * sizeof(char)));

    for (size_t i = 0; num == 0 || i < num; i++) {
        memtest_cacheEff_core(matrix, dim, sleepTime, logTag);
    }

    free(matrix);
}

void memtest_L1cacheEff(useconds_t sleepTime, size_t num, int cpu)
{
    if(cpu != -1) setCurrentThreadAffinity(cpu);
    memtest_cacheEff(sleepTime, 96, num, LOG_TAG_MEM_TEST_L1_CACHE_EFF); //  9 KB matrix < 16 KB L1 Cache
}

void memtest_L2cacheEff(useconds_t sleepTime, size_t num, int cpu)
{
    if(cpu != -1) setCurrentThreadAffinity(cpu);
    memtest_cacheEff(sleepTime, 1024, num, LOG_TAG_MEM_TEST_L2_CACHE_EFF); //  1 MB matrix < 2 MB L2 Cache
}

void memtest_cacheIneff(useconds_t sleepTime, size_t num, int cpu)
{
    if(cpu != -1) setCurrentThreadAffinity(cpu);

    const size_t dim = 2048; //  4 MB matrix > 2 MB L2 Cache

    char *matrix = reinterpret_cast<char*>(malloc(dim * dim * sizeof(char)));

    for (size_t i = 0; num == 0 || i < num; i++) {
        memtest_cacheEff_core(matrix, dim, sleepTime, LOG_TAG_MEM_TEST_CACHE_INEFF);
    }

    free(matrix);
}

void memtest_cacheCompare(useconds_t sleepTime, size_t dim, size_t num)
{
    const char* logTag = "MemTestCacheCompare";

    char *matrixEff = reinterpret_cast<char*>(malloc(dim * dim * sizeof(char)));
    char *matrixIneff = reinterpret_cast<char*>(malloc(dim * dim * sizeof(char)));

    for (size_t i = 0; i < num; i++) {
        memtest_cacheEff_core(matrixEff, dim, sleepTime, logTag);
    }

    printf("\n");

    for (size_t i = 0; i < num; i++) {
        memtest_cacheIneff_core(matrixIneff, dim, sleepTime, logTag);
    }

    free(matrixEff);
    free(matrixIneff);
}
