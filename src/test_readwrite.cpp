#include <stdlib.h>
#include "test_readwrite.h"

/************************************* READ *************************************/

/*
 * Verifies a block of memory contains an incrementing bit pattern.
 * To be called during irradiation and after
 */
int readTestCheck(char *matrix, size_t bytes)
{
    int errorCount = 0;

    char pattern = 0;
    for(int i = 0; i < bytes; i++)
    {
        if(pattern != matrix[i])
        {
            errorCount++;
        }
        pattern++;
    }

    return errorCount;
}


/************************************* WRITE *************************************/

/*
 * Initializes a block of memory with an incrementing bit pattern.
 * To be called before irradiation begins
 */
char* initIncrementing(size_t bytes)
{
    char *matrix = reinterpret_cast<char*>(malloc(bytes * sizeof(char)));

    char pattern = 0;
    for(int i = 0; i < bytes; i++)
    {
        matrix[i] = pattern;
        pattern++;
    }

    return matrix;
}

/*
 * Writes the one's complement of an incrementing bit pattern to the provided block of memory
 */
void writeIncrementingComplement(char *ptr, size_t bytes)
{
    char pattern = 0;
    for(int i = 0; i < bytes; i++){
        ptr[i] = ~pattern;
        pattern++;
    }
}

/*
 * Verifies a block of memory contains the inverse (one's complement) of an incrementing bit pattern
 */
int writeTestCheck(char* ptr, size_t bytes)
{
    int errorCount = 0;
    char pattern = 0;
    for(int i = 0; i < bytes; i++) {
        if(pattern != ~ptr[i]) errorCount++;
        pattern++;
    }

    return errorCount;
}


/************************************* MAIN *************************************/

void readWriteTest(size_t bytes, size_t iterations)
{
    char* readBlock = initIncrementing(bytes);
    char* writeBlock = initIncrementing(bytes);

    int count = 0;
    while(count < 1000)/*TODO input over USB*/
    {
        // TODO Wait for USB signal
        usleep(1000);
        count++;
    }

    // Radiation started
    writeIncrementingComplement(writeBlock, bytes);

    // Begin checking
    for(int i = 0; i < iterations; i++)
    {
        int readErrors = readTestCheck(readBlock, bytes);
        
    }
}