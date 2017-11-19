#ifndef RADTEST_ORBIT_LOGGING_H
#define RADTEST_ORBIT_LOGGING_H

#include <ctime>

namespace orbit
{

typedef unsigned int test_id_t;

class Logging
{

public:
    static const test_id_t TEST_MEM_L1_CACHE_EFF  = 0;
    static const test_id_t TEST_MEM_L2_CACHE_EFF  = 1;
    static const test_id_t TEST_MEM_CACHE_INEFF   = 2;
    static const test_id_t TEST_READ              = 3;
    static const test_id_t TEST_WRITE             = 4;
    static const test_id_t TEST_PERSIST           = 5;

    static Logging& getInstance();

    Logging();
    void logResult(test_id_t testId, const char *data, long double execTime, time_t timestamp);
    
    // delete copy and move constructors and assign operators
    Logging(Logging const&) = delete;             // Copy construct
    Logging(Logging&&) = delete;                  // Move construct
    Logging& operator=(Logging const&) = delete;  // Copy assign
    Logging& operator=(Logging &&) = delete;      // Move assign
};

}

#endif //RADTEST_ORBIT_LOGGING_H