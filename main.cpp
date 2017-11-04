#include "memtest.h"

int main()
{
    memtest_L1cacheEff(1000, 100, -1);
    memtest_L2cacheEff(1000, 100, -1);
    memtest_cacheIneff(1000, 100, -1);

    return 1;
}
