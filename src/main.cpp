#include "test_mem.h"

int main()
{
    //memtest_L1cacheEff(1000, 100, -1);
    //memtest_L2cacheEff(1000, 100, -1);
    //memtest_cacheIneff(1000, 100, -1);

    orbit::testMemCacheCompare(1000, 2048, 100);
}
