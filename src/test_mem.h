#ifndef RADTEST_TESTMEM_H
#define RADTEST_TESTMEM_H

#include <stdint.h>
#include <unistd.h>
#include "util/logging/log_queue.h"

namespace orbit
{

struct testMemResults_t
{
    size_t zeroToOneFlips;
    size_t oneToZeroFlips;
};

void testMemL1CacheEff(useconds_t sleepTime, size_t num, int cpu, LogQueue *logQueue);
void testMemL2CacheEff(useconds_t sleepTime, size_t num, int cpu, LogQueue *logQueue);
void testMemCacheIneff(useconds_t sleepTime, size_t num, int cpu, LogQueue *logQueue);
//void testMemCacheCompare(useconds_t sleepTime, size_t dim, size_t num);

}

#endif //RADTEST_TESTMEM_H
