#include "logging.h"

#include <iostream>

namespace orbit
{

void logResult(testResult_t result)
{
    printf("testId=%d, data=%s, execTime=%Lf, timestamp=%ld\n", result.testId, result.data, result.execTime, result.timestamp);
}

}