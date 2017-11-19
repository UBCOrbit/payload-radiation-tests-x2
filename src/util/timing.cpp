#include "timing.h"
#include <sys/time.h>
#include <chrono>

namespace orbit
{

timestamp_t getTimestamp()
{
    struct timeval now;
    gettimeofday(&now, nullptr);
    return now.tv_usec + (timestamp_t) now.tv_sec * 1000000;
}

long getEpochMillis()
{
    using namespace std::chrono;
    milliseconds timestamp = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    return timestamp.count();
}

}