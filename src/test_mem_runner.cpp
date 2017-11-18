#include "test_mem.h"

#include <pthread.h>
#include <iostream>

#define TEST_MEM_THREADS 3

namespace orbit
{

typedef struct testMemThreadData_t
{
    useconds_t sleepTime;
    size_t iterations;
    int cpu;
} testMemThreadData_t;

void *testMemL1ThreadFunc(void *arg)
{
    testMemThreadData_t *data = (testMemThreadData_t *)arg;

    orbit::testMemL1CacheEff((*data).sleepTime, (*data).iterations, (*data).cpu);
}

void *testMemL2ThreadFunc(void *arg)
{
    testMemThreadData_t *data = (testMemThreadData_t *)arg;

    orbit::testMemL2CacheEff((*data).sleepTime, (*data).iterations, (*data).cpu);
}

void *testMemIneffThreadFunc(void *arg)
{
    testMemThreadData_t *data = (testMemThreadData_t *)arg;

    orbit::testMemCacheIneff((*data).sleepTime, (*data).iterations, (*data).cpu);
}

int runTestMem()
{
    pthread_t threads[TEST_MEM_THREADS];

    testMemThreadData_t testMemThreadData[TEST_MEM_THREADS];

    // L1
    testMemThreadData[0].sleepTime = 1000;
    testMemThreadData[0].iterations = 10;
    testMemThreadData[0].cpu = 1;

    // L2
    testMemThreadData[1].sleepTime = 1000;
    testMemThreadData[1].iterations = 10;
    testMemThreadData[1].cpu = 2;

    // Main RAM
    testMemThreadData[2].sleepTime = 1000;
    testMemThreadData[2].iterations = 10;
    testMemThreadData[2].cpu = 3;

    int rc;
    if ((rc = pthread_create(&threads[0], NULL, testMemL1ThreadFunc, &testMemThreadData[0])))
    {
        fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
        return EXIT_FAILURE;
    }

    if ((rc = pthread_create(&threads[1], NULL, testMemL2ThreadFunc, &testMemThreadData[1])))
    {
        fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
        return EXIT_FAILURE;
    }

    if ((rc = pthread_create(&threads[2], NULL, testMemIneffThreadFunc, &testMemThreadData[2])))
    {
        fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
        return EXIT_FAILURE;
    }

    for(int i = 0; i < TEST_MEM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
}

}