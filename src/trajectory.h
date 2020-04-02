#ifndef __YAL_TRAJECTORY
#define __YAL_TRAJECTORY

namespace Yal {

enum {
  TRJ_JERKMIN,
  TRJ_LINEAR,
  TRJ_ACCEL,
};

template<class T>
class PPTrajectory {
public:
  double interval;
  double tdes;
  double torig;
  double time;
  T des;
  T orig;
  T L;
  int type;

  PPTrajectory() : interval(1.0), torig(0.0), type(0){}

  void create(const T& des2, const T& orig2, double tdes2, double torig2, int type2=0) {
    type = type2;
    orig = orig2;
    des = des2;
    tdes = tdes2;
    interval = tdes;
    torig = torig2;
    L = (des-orig);
  }
  void modify_destination(const T& des2, const T& orig2){
    des = des2;
    orig = orig2;
    torig = time;
    interval = tdes-time;
    L = (des-orig);
  }

  T get(double t, bool* flag=0) {
    time = t;
    double ts = (time-torig) / interval; //時間の規格化

    if (ts > 1.0) { //到着
      if (flag) * flag = false;
      return des;
    } else if (ts < 0.0) { //出発前
      if (flag) * flag = true;
      return orig;
    }

    if (flag) * flag = true; // 移動中

    switch(type) {
    case TRJ_JERKMIN:
      return -L * ( 15*pow(ts,4) - 6*pow(ts,5) - 10*pow(ts,3) ) + orig;
      break;

    case TRJ_ACCEL:
      {
        const double Ta2 = 0.1;
        const T V2 = L / (1.0 - Ta2);
        const T A = 2.0/3 * V2 /Ta2/Ta2;
    
        if (ts < 0.5*Ta2)
          return A * pow(ts,3) + orig;
        else if (ts < Ta2)
          return - A * pow(ts-Ta2, 3) + V2 * ts - 1.0/2 * V2 * Ta2 + orig;
        else if (ts < 1.0 - Ta2)
          return  V2 * ts - 1.0/2* V2 * Ta2 + orig;
        else if (ts < 1.0 - 0.5*Ta2)
          return - A * pow(ts-(1.0-Ta2),3) + V2 * ts - 1.0/2 * V2 * Ta2 + orig;
        else //if( ts< 1.0 )
          return  A * pow(ts-1.0,3) + des;
      }
      break;
      
    case TRJ_LINEAR: //LINEAR
      return L * ts + orig;
      break;
    }
  }
};

}

#endif
