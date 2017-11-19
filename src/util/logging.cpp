#include "logging.h"
#include <iostream>

namespace orbit
{

Logging& Logging::getInstance()
{
    static Logging INSTANCE;
    return INSTANCE;
}

Logging::Logging()
{
    // Nothing to construct
}

void logResult(test_id_t testId, const char *data, long double execTime, time_t timestamp)
{
    printf("testId = %d, data = %s, execTime = %Lf, timestamp=%Ld\n", testId, data, execTime, timestamp);
}

// void Logging::logResult(const char *testId, const char *data, long double time)
// {
//     printf("testId = %s, data = %s, time = %Lf\n", testId, data, time);
// }

}