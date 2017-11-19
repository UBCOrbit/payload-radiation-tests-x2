#ifndef RADTEST_ORBIT_LOG_QUEUE_H
#define RADTEST_ORBIT_LOG_QUEUE_H

#include <pthread.h>
#include <queue>

namespace orbit
{

typedef unsigned int test_id_t;

typedef struct
{
    test_id_t testId;
    char data[128]; // max 127 characters
    long double execTime;
    long timestamp; // millis
} testResult_t;

class LogQueue
{
    std::queue<testResult_t> queue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;

public:
    static const test_id_t TIMED_OUT              = -1;

    static const test_id_t TEST_MEM_L1_CACHE_EFF  = 0;
    static const test_id_t TEST_MEM_L2_CACHE_EFF  = 1;
    static const test_id_t TEST_MEM_CACHE_INEFF   = 2;
    static const test_id_t TEST_READ              = 3;
    static const test_id_t TEST_WRITE             = 4;
    static const test_id_t TEST_PERSIST           = 5;

    LogQueue();
    ~LogQueue();

    void enqueueTestResult(testResult_t resultData);
    testResult_t dequeueTestResult(int timeoutMillis);
};

}

#endif //RADTEST_ORBIT_LOG_QUEUE_H