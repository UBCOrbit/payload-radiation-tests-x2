#include "test_mem_runner.h"

#include "test_mem.h"

#include <pthread.h>
#include <iostream>

void *testMemL1ThreadFunc(void *arg)
{
    testMemThreadData_t *data = (testMemThreadData_t *)arg;

    orbit::testMemL1CacheEff(data->sleepTime, data->iterations, data->cpu, data->logQueue);

    data->complete = true;
    pthread_exit(0);
}

void *testMemL2ThreadFunc(void *arg)
{
    testMemThreadData_t *data = (testMemThreadData_t *)arg;

    orbit::testMemL2CacheEff(data->sleepTime, data->iterations, data->cpu, data->logQueue);

    data->complete = true;
    pthread_exit(0);
}

void *testMemIneffThreadFunc(void *arg)
{
    testMemThreadData_t *data = (testMemThreadData_t *)arg;

    orbit::testMemCacheIneff(data->sleepTime, data->iterations, data->cpu, data->logQueue);

    data->complete = true;
    pthread_exit(0);
}