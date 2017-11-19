#include "result_queue.h"

#include <iostream>
#include <sys/time.h>

namespace orbit
{

ResultQueue::ResultQueue()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
}

ResultQueue::~ResultQueue()
{
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}

void ResultQueue::enqueueTestResult(testResult_t resultData)
{
    pthread_mutex_lock(&mutex);
    queue.push(resultData);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

testResult_t ResultQueue::dequeueTestResult(int timeoutMillis)
{
    struct timeval tv;
    struct timespec ts;

    gettimeofday(&tv, NULL);
    ts.tv_sec = time(NULL) + timeoutMillis / 1000;
    ts.tv_nsec = tv.tv_usec * 1000 + 1000 * 1000 * (timeoutMillis % 1000);
    ts.tv_sec += ts.tv_nsec / (1000 * 1000 * 1000);
    ts.tv_nsec %= (1000 * 1000 * 1000);

    pthread_mutex_lock(&mutex);
    
    int waitResult = 0;
    while(queue.size() == 0)
    {
        waitResult = pthread_cond_timedwait(&cond, &mutex, &ts);

        if(waitResult == ETIMEDOUT)
        {
            break;
        }
    }

    testResult_t result;
    if(waitResult == ETIMEDOUT)
    {
        result.testId = TIMED_OUT;
    }
    else
    {
        result = queue.front();
        queue.pop();
    }

    pthread_mutex_unlock(&mutex);

    return result;
}

}