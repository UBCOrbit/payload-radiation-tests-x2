#ifndef orbit_output_h_INCLUDED
#define orbit_output_h_INCLUDED

#include <stdio.h>

#ifdef NDEBUG
// TODO: Output for when running tests
#else
#define LOGD(format, args...) printf(format, args)
#define LOGE(format, args...) fprintf(stderr, format, args)
#endif // NDEBUG

#endif // orbit_output_h_INCLUDED
