#include <iostream>
#include <stdio.h>      // standard input / output functions
#include <stdlib.h>
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions

#include "test_mem_runner.h"
#include "test_readwrite.h"
#include "util/output/result_queue.h"
#include "util/output/output.h"
#include "util/timing.h"
#include "util/serial.h"

#define TEST_MEM_THREAD_COUNT 3

#define HEARTBEAT_TIMEOUT 5e6 // microseconds

#define SIGNAL_RADIATION "RAD"
#define SIGNAL_RADIATION_LEN 3

int runTestMem(pthread_t *threads, testMemThreadData_t *l1CacheEffThreadArg, testMemThreadData_t *l2CacheEffThreadArg, testMemThreadData_t *cacheIneffThreadArg)
{
    int rc;
    if ((rc = pthread_create(&threads[0], NULL, testMemL1ThreadFunc, l1CacheEffThreadArg)))
    {
        fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
        return EXIT_FAILURE;
    }

    if ((rc = pthread_create(&threads[1], NULL, testMemL2ThreadFunc, l2CacheEffThreadArg)))
    {
        fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
        return EXIT_FAILURE;
    }

    if ((rc = pthread_create(&threads[2], NULL, testMemIneffThreadFunc, cacheIneffThreadArg)))
    {
        fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
        return EXIT_FAILURE;
    }
    return 0;
}

typedef struct
{
    int *device;
} serialMonitorArg_t;

void *waitForRadStart(void *arg)
{
    serialMonitorArg_t *data = (serialMonitorArg_t *)arg;
    
    int n = 0,
    spot = 0;
    char buf = '\0';

    /* Whole response*/
    char response[1024];
    memset(response, '\0', sizeof response);

    while(n > 0)
    {
        spot = 0; // Reset in buffer

        do
        {
            n = read(*(data->device), &buf, 1 );
            sprintf(&response[spot], "%c", buf);
            spot += n;
        }
        while(buf != '\n' && n > 0);

        for(int i = 0; i < SIGNAL_RADIATION_LEN; i++)
        {
            if(response[i] == SIGNAL_RADIATION[i]) break;
        }
    }

    pthread_exit(0);
}

typedef struct
{
    pthread_t *radStartMonitor;
} readWriteTestThreadArg_t;


void *readWriteTestThread(void *arg)
{
    readWriteTestThreadArg_t *data = (readWriteTestThreadArg_t *)arg;
    
    // Wait for rad start
    pthread_join(*(data->radStartMonitor), NULL);

    printf("RAD TEST STARTED");
}

int main(int argc, char *argv[])
{
    int iterations = 0;
    if(argc > 1)
    {
        iterations = atoi(argv[1]);
    }

    orbit::ResultQueue resultQueue;

    // Create memory test threads
    pthread_t threads[TEST_MEM_THREAD_COUNT];

    testMemThreadData_t *l1CacheEffThreadArg = (testMemThreadData_t*)malloc(sizeof(*l1CacheEffThreadArg));
    l1CacheEffThreadArg->sleepTime = 1000;
    l1CacheEffThreadArg->iterations = iterations;
    l1CacheEffThreadArg->cpu = 1;
    l1CacheEffThreadArg->resultQueue = &resultQueue;
    l1CacheEffThreadArg->complete = false;
    
    testMemThreadData_t *l2CacheEffThreadArg = (testMemThreadData_t*)malloc(sizeof(*l2CacheEffThreadArg));
    l2CacheEffThreadArg->sleepTime = 1000;
    l2CacheEffThreadArg->iterations = iterations;
    l2CacheEffThreadArg->cpu = 2;
    l2CacheEffThreadArg->resultQueue = &resultQueue;
    l2CacheEffThreadArg->complete = false;
    
    testMemThreadData_t *cacheIneffThreadArg = (testMemThreadData_t*)malloc(sizeof(*cacheIneffThreadArg));
    cacheIneffThreadArg->sleepTime = 1000;
    cacheIneffThreadArg->iterations = iterations;
    cacheIneffThreadArg->cpu = 3;
    cacheIneffThreadArg->resultQueue = &resultQueue;
    cacheIneffThreadArg->complete = false;

    int testMemResult = runTestMem(threads, l1CacheEffThreadArg, l2CacheEffThreadArg, cacheIneffThreadArg);

    int *device;
    orbit::setup(device);

    serialMonitorArg_t serialMonitorThreadArg;
    serialMonitorThreadArg.device = device;
    pthread_t radStartThread;
    int rc;
    if ((rc = pthread_create(&radStartThread, NULL, waitForRadStart, l2CacheEffThreadArg)))
    {
        fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
        return EXIT_FAILURE;
    }

    orbit::outputHeartbeat();
    orbit::timestamp_t lastHeartbeat = orbit::getTimestamp();

    while(true)
    {
        orbit::testResult_t result = resultQueue.dequeueTestResult(5000);
        if(result.testId == orbit::ResultQueue::TIMED_OUT)
        {
            // Check if all threads are complete
            if(l1CacheEffThreadArg->complete && l2CacheEffThreadArg->complete && cacheIneffThreadArg->complete)
            {
                break;
            }

            printf("Timed Out\n");
        }
        else
        {
            orbit::outputResult(result);
            // TODO log to file and to ttyTHS2

            orbit::timestamp_t currentTimestamp = orbit::getTimestamp();
            if((currentTimestamp - lastHeartbeat) > HEARTBEAT_TIMEOUT)
            {
                lastHeartbeat = currentTimestamp;
                orbit::outputHeartbeat();
            }
        }
    }

    free(l1CacheEffThreadArg);
    free(l2CacheEffThreadArg);
    free(cacheIneffThreadArg);
}
