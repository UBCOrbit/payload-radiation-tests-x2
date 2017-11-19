#include <iostream>

#include "test_mem_runner.h"
#include "util/logging/log_queue.h"
#include "util/logging/logging.h"

#define TEST_MEM_THREAD_COUNT 1

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

int main()
{
    orbit::LogQueue logQueue;

    // Create memory test threads
    pthread_t threads[TEST_MEM_THREAD_COUNT];

    testMemThreadData_t *l1CacheEffThreadArg = (testMemThreadData_t*)malloc(sizeof(*l1CacheEffThreadArg));
    l1CacheEffThreadArg->sleepTime = 1000;
    l1CacheEffThreadArg->iterations = 10;
    l1CacheEffThreadArg->cpu = 1;
    l1CacheEffThreadArg->logQueue = &logQueue;
    l1CacheEffThreadArg->complete = false;
    
    testMemThreadData_t *l2CacheEffThreadArg = (testMemThreadData_t*)malloc(sizeof(*l2CacheEffThreadArg));
    l2CacheEffThreadArg->sleepTime = 1000;
    l2CacheEffThreadArg->iterations = 10;
    l2CacheEffThreadArg->cpu = 2;
    l2CacheEffThreadArg->logQueue = &logQueue;
    l2CacheEffThreadArg->complete = false;
    
    testMemThreadData_t *cacheIneffThreadArg = (testMemThreadData_t*)malloc(sizeof(*cacheIneffThreadArg));
    cacheIneffThreadArg->sleepTime = 1000;
    cacheIneffThreadArg->iterations = 10;
    cacheIneffThreadArg->cpu = 3;
    cacheIneffThreadArg->logQueue = &logQueue;
    cacheIneffThreadArg->complete = false;

    int testMemResult = runTestMem(threads, l1CacheEffThreadArg, l2CacheEffThreadArg, cacheIneffThreadArg);

    while(true)
    {
        orbit::testResult_t result = logQueue.dequeueTestResult(5000);
        if(result.testId == orbit::LogQueue::TIMED_OUT)
        {
            printf("Timed Out\n");
        }
        else
        {
            orbit::logResult(result);
        }
    }

    free(l1CacheEffThreadArg);
    free(l2CacheEffThreadArg);
    free(cacheIneffThreadArg);
}
