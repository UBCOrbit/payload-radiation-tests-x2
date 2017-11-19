#ifndef RADTEST_TEST_MEM_RUNNER_H
#define RADTEST_TEST_MEM_RUNNER_H

#include "test_mem.h"
#include "util/logging/log_queue.h"

typedef struct
{
    useconds_t sleepTime;
    size_t iterations;
    int cpu;
    orbit::LogQueue *logQueue;
    volatile bool complete;
} testMemThreadData_t;

void *testMemL1ThreadFunc(void *arg);
void *testMemL2ThreadFunc(void *arg);
void *testMemIneffThreadFunc(void *arg);


#endif // RADTEST_TEST_MEM_RUNNER_H