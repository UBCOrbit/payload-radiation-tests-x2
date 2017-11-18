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

void Logging::logResult(const char *testId, const char *data, long double time)
{
    printf("testId = %s, data = %s, time = %Lf\n", testId, data, time);
}

}