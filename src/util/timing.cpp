#include "timing.h"
#include <sys/time.h>

namespace orbit
{

timestamp_t get_timestamp()
{
    struct timeval now;
    gettimeofday(&now, nullptr);
    return now.tv_usec + (timestamp_t) now.tv_sec * 1000000;
}

}