#include<stdio.h>
#include<stdarg.h>
#include<stdlib.h>
#include<math.h>
#include"timer.h"

using namespace Yal;

struct timespec GlobalTimer::initial;
struct timespec GlobalTimer::current;
struct timespec GlobalTimer::previous;



