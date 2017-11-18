#ifndef RADTEST_ORBIT_LOGGING_H
#define RADTEST_ORBIT_LOGGING_H

namespace orbit
{

class Logging
{

public:
    static Logging& getInstance();

    Logging();
    void logResult(const char *testId, const char *data, long double time);
    
    // delete copy and move constructors and assign operators
    Logging(Logging const&) = delete;             // Copy construct
    Logging(Logging&&) = delete;                  // Move construct
    Logging& operator=(Logging const&) = delete;  // Copy assign
    Logging& operator=(Logging &&) = delete;      // Move assign

};

}

#endif //RADTEST_ORBIT_LOGGING_H