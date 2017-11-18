#ifndef RADTEST_CPU_AFFINITY_H
#define RADTEST_CPU_AFFINITY_H

namespace orbit
{

int setCurrentThreadAffinity(int cpuNum);

}

#endif //RADTEST_CPU_AFFINITY_H