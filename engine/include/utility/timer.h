#pragma once

#if defined(WIN32)
#include <windows.h>
#else
#include <sys/time.h>
#endif

class Timer
{
public:
    Timer();
    ~Timer();
    
    void   start();
    void   stop();
    double get_elapsed_time();
    double get_elapsed_time_sec();
    double get_elapsed_time_milisec();
    double get_elapsed_time_microsec();
    
private:
    double _start_time_microsec;
    double _end_time_microsec;
    int    _stopped;
#ifdef WIN32
    LARGE_INTEGER _frequency;
    LARGE_INTEGER _start_count;
    LARGE_INTEGER _end_count;
#else
    timeval _start_count;
    timeval _end_count;
#endif
};
