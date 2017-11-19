#ifndef RADTEST_TEST_MEM_H
#define RADTEST_TEST_MEM_H

#include <stdint.h>
#include <unistd.h>
#include "util/output/result_queue.h"

namespace orbit
{

struct testMemResults_t
{
    size_t zeroToOneFlips;
    size_t oneToZeroFlips;
};

void testMemL1CacheEff(useconds_t sleepTime, size_t num, int cpu, ResultQueue *resultQueue);
void testMemL2CacheEff(useconds_t sleepTime, size_t num, int cpu, ResultQueue *resultQueue);
void testMemCacheIneff(useconds_t sleepTime, size_t num, int cpu, ResultQueue *resultQueue);
//void testMemCacheCompare(useconds_t sleepTime, size_t dim, size_t num);

}

#endif //RADTEST_TEST_MEM_H
