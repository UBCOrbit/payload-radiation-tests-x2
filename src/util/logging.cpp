#include "logging.h"
#include <iostream>

namespace orbit
{

void logResult(const char *testId, const char *data, long double time)
{
    printf("testId = %s, data = %s, time = %Lf\n", testId, data, time);
}

}