#ifndef __YAL_KERNEL
#define __YAL_KERNEL

#include<stdio.h>
#include<math.h>

// #ifndef __BEGIN_DECLS
// #ifdef __cplusplus
// #define __BEGIN_DECLS extern "C"{
// #define __END_DECLS }
// #else
// #define __BEGIN_DECLS
// #define __END_DECLS
// #endif
// #endif

#define Fc 1  //カットオフ

#ifndef Ts
#define Ts 0.001  // サンプリング時間
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef PI
#define PI M_PI
#define M_2PI  6.283185307179586232
#endif

#define SQRT3_2 0.86602540378443859659

namespace Yal {

// __BEGIN_DECLS

#ifndef __KERNEL__
enum{HMSD,HMS,MSD,MS,SD};
extern const char* formatTime(double t,int ftype=HMSD);
extern void shell(const char *format, ...);
extern FILE *popenf(const char *format,const char *rw, ...);
extern double get_cpufreq(); //only user space
#endif

// __END_DECLS

}

#endif
