#include "output.h"

#include <iostream>

#define SIGNAL_HEARTBEAT "HEARTBEAT\n"

namespace orbit
{

void outputResult(testResult_t result)
{
    printf("testId=%d, data=%s, execTime=%Lf, timestamp=%ld\n", result.testId, result.data, result.execTime, result.timestamp);
}

void outputHeartbeat()
{
    printf(SIGNAL_HEARTBEAT);
}

}