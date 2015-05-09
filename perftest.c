#include "perftest.h"
#include "stdio.h"
#include "sys/time.h"
#include "stdlib.h"

int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;
    
    return (diff<0);
}

void timeval_print(struct timeval *tv)
{
    char buffer[30];
    time_t curtime;
    
    fprintf(stderr, "%ld.%06i", tv->tv_sec, tv->tv_usec);
    curtime = tv->tv_sec;
    strftime(buffer, 30, "%m-%d-%Y  %T", localtime(&curtime));
    fprintf(stderr," = %s.%06i\n", buffer, tv->tv_usec);
}

// beginoftest
PerfTest * PTstart()
{
    PerfTest * pt = (PerfTest*)malloc(sizeof(PerfTest));
    if (!pt)
    {
        fprintf(stderr,"couldn't allocate mem for perftest\n");
        return NULL;
    }
    gettimeofday(&pt->tvBegin, NULL);
    timeval_print(&pt->tvBegin);
    
    return pt;
}

//return seconds since start
double PTinterval(PerfTest * pt)
{
    gettimeofday(&pt->tvEnd, NULL);
    timeval_print(&pt->tvEnd);

    timeval_subtract(&pt->tvDiff, &pt->tvEnd, &pt->tvBegin);
    return pt->tvDiff.tv_sec+(double)(pt->tvDiff.tv_usec/1000000.0);
}
