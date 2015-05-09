#include "unistd.h"

typedef struct
{
    struct timeval tvBegin, tvEnd, tvDiff;
} PerfTest;

//starts a performance test and returns the created data structure
PerfTest * PTstart();

//return seconds since start
double PTinterval(PerfTest * pt);
