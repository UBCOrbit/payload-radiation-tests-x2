#ifndef RADTEST_MEMTEST_H
#define RADTEST_MEMTEST_H

#include <stdint.h>
#include <unistd.h>

#define LOG_TAG_MEM_TEST_L1_CACHE_EFF "MemTestL1CacheEff"
#define LOG_TAG_MEM_TEST_L2_CACHE_EFF "MemTestL2CacheEff"
#define LOG_TAG_MEM_TEST_CACHE_INEFF "MemTestCacheIneff"

struct memtest_results
{
    size_t zeroToOneFlips;
    size_t oneToZeroFlips;
};

void memtest_L1cacheEff(useconds_t sleepTime, size_t num, int cpu);
void memtest_L2cacheEff(useconds_t sleepTime, size_t num, int cpu);
void memtest_cacheIneff(useconds_t sleepTime, size_t num, int cpu);
void memtest_cacheCompare(useconds_t sleepTime);

#endif //RADTEST_MEMTEST_H
