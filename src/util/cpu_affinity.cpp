#include "cpu_affinity.h"

#include <stdio.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <sched.h>

namespace orbit
{

/*void setCurrentThreadAffinityMask(cpu_set_t mask)
{
    long err, syscallres;
    pid_t pid = getgid();
    syscallres = syscall(__NR_sched_setaffinity, pid, sizeof(mask), &mask);
    if (syscallres)
    {
        err = errno;
        fprintf(stderr, "Error in the syscall setaffinity: mask=%d=0x%x err=%ld=0x%lx", mask, mask, err, err);
    }
}

void setCurrentThreadAffinity(int cpuNum)
{
    cpu_set_t set;

    CPU_ZERO(&set);
    CPU_SET(cpuNum, &set);

    setCurrentThreadAffinityMask(set);
}*/

int setCurrentThreadAffinity(int cpuNum)
{
    int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    if (cpuNum < 0 || cpuNum >= num_cores)
    {
        return EINVAL;
    }

   cpu_set_t cpuset;
   CPU_ZERO(&cpuset);
   CPU_SET(cpuNum, &cpuset);

   pthread_t current_thread = pthread_self();    
   return pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
}

}
