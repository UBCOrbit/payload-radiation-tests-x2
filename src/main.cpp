#include <pthread.h>
#include <iostream>

#include "test_mem.h"

#define NUM_THREADS 3

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

int main()
{
    pthread_t testMemL1Thread;
    pthread_t testMemL2Thread;
    pthread_t testMemIneffThread;

    testMemThreadData_t testMemThreadData[1];
    testMemThreadData[0].sleepTime = 1000;
    testMemThreadData[0].iterations = 10;
    testMemThreadData[0].cpu = -1;

    int rc;
    if ((rc = pthread_create(&testMemL1Thread, NULL, testMemL1ThreadFunc, &testMemThreadData[0])))
    {
        fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
        return EXIT_FAILURE;
    }

    if ((rc = pthread_create(&testMemL2Thread, NULL, testMemL2ThreadFunc, &testMemThreadData[0])))
    {
        fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
        return EXIT_FAILURE;
    }

    if ((rc = pthread_create(&testMemIneffThread, NULL, testMemIneffThreadFunc, &testMemThreadData[0])))
    {
        fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
        return EXIT_FAILURE;
    }

    pthread_join(testMemL1Thread, NULL);
    pthread_join(testMemL2Thread, NULL);
    pthread_join(testMemIneffThread, NULL);
}
