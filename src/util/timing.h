#ifndef RADTEST_TIMING_H
#define RADTEST_TIMING_H

namespace orbit
{

typedef unsigned long long timestamp_t;

timestamp_t getTimestamp();

long getEpochMillis();

}

#endif //RADTEST_TIMING_H
