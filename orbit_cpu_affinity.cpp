#include "orbit_output.h"

#include <sys/syscall.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

void setCurrentThreadAffinityMask(cpu_set_t mask)
{
    long err, syscallres;
    pid_t pid = getgid();
    syscallres = syscall(__NR_sched_setaffinity, pid, sizeof(mask), &mask);
    if (syscallres)
    {
        err = errno;
        LOGE("Error in the syscall setaffinity: mask=%d=0x%x err=%ld=0x%lx", mask, mask, err, err);
    }
}

void setCurrentThreadAffinity(int cpuNum)
{
    cpu_set_t set;

    CPU_ZERO(&set);
    CPU_SET(cpuNum, &set);

    setCurrentThreadAffinityMask(set);
}
