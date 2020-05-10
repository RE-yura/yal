#ifndef __YAL_TIMER
#define __YAL_TIMER

#include<stdio.h>
#include<time.h>
#include<sys/resource.h>

namespace Yal {

class Timer {
  struct timespec initial;
  struct timespec current;
  struct timespec previous;
  
 public:
  void reset() {//double t0=0.0){
    //clock_gettime(CLOCK_MONOTONIC, &current);
    clock_gettime(CLOCK_MONOTONIC_RAW, &current);
    initial = previous = current;
    initial = current;
  }
  
  double get_time() {
    previous = current;
    
    //clock_gettime(CLOCK_MONOTONIC, &current);
    clock_gettime(CLOCK_MONOTONIC_RAW, &current);
    return current.tv_sec - initial.tv_sec + (current.tv_nsec - initial.tv_nsec)*1e-9; //[s]
  }

  double get_interval() const {
    return current.tv_sec - previous.tv_sec + (current.tv_nsec - previous.tv_nsec)*1e-9; //[s]
  }
};


class GlobalTimer {
  static struct timespec initial;
  static struct timespec current;
  static struct timespec previous;
  
 public:
  static void reset() {
    //clock_gettime(CLOCK_MONOTONIC, &current);
    clock_gettime(CLOCK_MONOTONIC_RAW, &current);
    initial = previous = current;
    initial = current;
  }
  
  static double get_time() {
    previous = current;
    
    //clock_gettime(CLOCK_MONOTONIC, &current);
    clock_gettime(CLOCK_MONOTONIC_RAW, &current);
    return current.tv_sec - initial.tv_sec + (current.tv_nsec - initial.tv_nsec)*1e-9; //[s]
  }
  
  static double get_interval() {
    return current.tv_sec - previous.tv_sec + (current.tv_nsec - previous.tv_nsec)*1e-9; //[s]
  }
};

}

#endif
