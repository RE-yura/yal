#ifndef __YAL_UTIL
#define __YAL_UTIL
#include<stdio.h>
#include<math.h>

#ifndef __BEGIN_DECLS
#ifdef __cplusplus
#define __BEGIN_DECLS extern "C"{
#define __END_DECLS }
#else
#define __BEGIN_DECLS
#define __END_DECLS
#endif
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef PI
#define PI M_PI
#define M_2PI  6.283185307179586232
#endif

#define SQRT3_2 0.86602540378443859659
/** Kotaro Tadano's Library
 */
namespace Yal{
__BEGIN_DECLS

#ifndef __KERNEL__
enum{HMSD,HMS,MSD,MS,SD};
extern const char* formatTime(double t,int ftype=HMSD);

extern void shell(const char *format, ...);
extern FILE *popenf(const char *format,const char *rw, ...);
extern double get_cpufreq(); //only user space

#endif

/** 変数を範囲内に制限する.
 * @param value 対象変数のポインタ
 * @param min 下限値
 * @param max 上限値
 * @return 対象変数が既に範囲内にある場合は0,
 minより小さい場合は-1, maxより大きい場合は1
 */
inline int limitRange(double *value,
		      const double min,const double max ){
  if     ( *value < min ){
    *value = min;
    return -1;
  }
  else if( *value > max ){
    *value = max;
    return 1;
  }
  return 0;
}

/** 変数の正負を判定する.
 * @param x 対象
 * @return xが正であれば 1, 負であれば-1
 */
inline int sgn(double x){
  if( fabs(x) < 1e-9 )
    return 0;
  else if( x > 0 ) 
    return 1;
  else
    return -1;
}
/*
inline double threshold_filter(double x,double threshold,int n=1){
  if( threshold < 1e-6 ) return x;
  const double r = pow( fabs(x)/threshold, n );
  return x* ( 2.0 / ( 1.0 + exp( -r ) ) -1 ); // sigmoid
  }
*/
inline double threshold_filter(double x,double threshold,int n){
  //const double x_abs = fabs(x);
  if( threshold == 0.0 ){
    fprintf(stderr,"Yal::threshold_filter: th must be non-zero value\n");
    return x;
  }
  const double r = fabs(x) / threshold;
  return ( r < 1.0 ) ?  pow( r , n ) * x : x;

}

inline double threshold2(double x,double threshold,int n){
  const double x_abs = fabs(x);
  return ( x_abs < threshold ) ?
    pow( x_abs/threshold , n ) * x : x;
}

__END_DECLS


template<class T>
inline T get_min(const T& a, const T& b){
  if( a < b ) return a;
  else        return b;
}
template<class T>
inline T get_max(const T& a, const T& b){
  if( a > b ) return a;
  else        return b;

}

template<typename T>
class Range{
  T vmin;
  T vmax;
 public:
  Range(){}


 Range(T m,T p) :
  vmin(m), vmax(p){}
  void set(const T& m,const T& p){
    vmin=m; vmax=p;
  }
  void set_center(const T& center, const T& range){
    vmin = center - range;
    vmax = center + range;
  }
  T center()const{
    return (vmax + vmin) * 0.5;
  }
  int check(const T& value)const{
    if( value < vmin )      return -1;
    else if( value > vmax ) return 1;
    else                   return 0;
  }
  int limit(T& value)const{
    if     ( value < vmin ){
      value = vmin;
      return -1;
    }
    else if( value > vmax ){
      value = vmax;
      return 1;
    }
    return 0;
  }
  T over(const T& value)const{
    if     ( value < vmin )
      return  value - vmin;
    else if( value > vmax )
      return value - vmax;
    return 0;
  }
  void expand(double r,int pivot=0){
    T new_range = r*(vmax - vmin);
    //range *= r;
    switch(pivot){
    case 0:{ // center
      T sum = (vmin+vmax);
      vmin = (sum - new_range) / 2;
      vmax = (sum + new_range) / 2;
    }
      break;
    case 1: //top
      vmin    = vmax - new_range;
      break;
    case -1: //bottom
      vmax    = vmin + new_range;
      break;
    }
  }
  void shift(const T& v){
    vmin += v;
    vmax += v;
  }
  /*
  const T& get_min()const{ return vmin; }
  const T& get_max()const{ return vmax; }
  T get_width()const{ return vmax-vmin; }
  */
  const T& min()const{ return vmin; }
  const T& max()const{ return vmax; }
  T width()const{ return vmax-vmin; }

  T normalize(const T& v){
    return (v-vmin) / (vmax-vmin);
  }

};

 enum {
   TRJ_JERKMIN,
   TRJ_LINEAR,
   TRJ_ACCEL,
 };

template<class T>
class PPTrajectory{
public:
  double interval;
  double tdep;
  T des;
  T dep;
  T L;
  int type;

  PPTrajectory() : interval(1.0), tdep(0.0), type(0){
    
  }

  void create(const T& des2, const T& dep2, double T2, double tdep2, int t=0){
    type = t;
    dep = dep2;
    des = des2;
    interval   = T2;
    tdep   = tdep2;
    L = (des-dep);
  }
  void modify_destination(const T& des2){
    des = des2;
    L = (des-dep);
  }

  T get(double t, bool* flag=0){
    double ts = (t-tdep)/interval; //時間の規格化

    if( ts > 1.0 ){ //到着
      if( flag ) *flag = false;
      return des;
    }
    else if( ts < 0.0 ){ //出発前
      if( flag ) *flag = true;
      return dep;
    }

    if( flag ) *flag = true; // 移動中

    switch(type){
    case TRJ_JERKMIN:

      return  -L * ( 15*pow(ts,4) - 6*pow(ts,5) - 10*pow(ts,3) ) + dep;
      break;

    case TRJ_ACCEL:

      {
	const double Ta2 = 0.1;
	const T V2 = L / (1.0 - Ta2);
	const T A = 2.0/3 * V2 /Ta2/Ta2;
	
	//if( ts < 0.0 ) 	  return dep;
	if( ts < 0.5* Ta2 )
	  return A * pow(ts,3) + dep;
	else if( ts < Ta2 )
	  return - A * pow(ts-Ta2, 3) + V2 * ts
	    - 1.0/2 * V2 * Ta2 + dep;
	else if( ts < 1.0 - Ta2 )
	  return  V2 * ts - 1.0/2* V2 * Ta2 + dep;
	else if( ts < 1.0 - 0.5*Ta2 )
	  return - A * pow(ts-(1.0-Ta2),3) + V2 * ts
	    - 1.0/2 * V2 * Ta2 + dep;
	else //if( ts< 1.0 )
	  return  A * pow(ts-1.0,3) + des;
	//else	  return des;
      }
      break;
      
      case TRJ_LINEAR: //LINEAR

      return L * ts + dep;
      break;
    }
      
  }
};

}

#endif
