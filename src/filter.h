#ifndef __YAL_FILTER
#define __YAL_FILTER

#include<stdio.h>
#include<math.h>

#define Fc 1  //カットオフ

#ifndef Ts
#define Ts 0.001  // サンプリングタイム
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

/** 変数値を所望の範囲内に制限
 * @param value 対象変数のポインタ
 * @param min 下限値
 * @param max 上限値
 * @return 対象変数が既に範囲内にある場合は0,
 minより小さい場合は-1, maxより大きい場合は1
 */
inline int limitRange(double& value, const double& min,const double& max) {
  if (value < min) {
    value = min;
    return -1;
  } else if (value > max) {
    value = max;
    return 1;
  }
  return 0;
}

/** 変数の正負を判定する.
 * @param x 対象
 * @return xが正であれば 1, 負であれば-1
 */
inline int sgn(const double& x) {
  if (fabs(x) < 1e-9)
    return 0;
  else if (x > 0) 
    return 1;
  else
    return -1;
}

/** 閾値フィルタ
 *
 */
inline double threshold(double& x, const double& threshold, const int& n){
  const double x_abs = fabs(x);
  return ( x_abs < threshold ) ? pow( x_abs/threshold , n ) * x : x;
}


template<class T>
inline T get_min(const T& a, const T& b) {
  if(a < b) return a;
  else      return b;
}
template<class T>
inline T get_max(const T& a, const T& b) {
  if(a > b) return a;
  else      return b;
}

template<typename T>
class Range {
  T vmin;
  T vmax;

 public:
  Range(){}
  Range(T m,T p) : vmin(m), vmax(p){}

  void set(const T& m, const T& p) {
    vmin=m; 
    vmax=p;
  }
  void set_center(const T& center, const T& range) {
    vmin = center - range;
    vmax = center + range;
  }
  T center()const {
    return (vmax + vmin) * 0.5;
  }
  int check(const T& value)const {
    if (value < vmin)      return -1;
    else if (value > vmax) return 1;
    else                   return 0;
  }
  int limit(T& value)const {
    if (value < vmin) {
      value = vmin;
      return -1;
    } else if (value > vmax) {
      value = vmax;
      return 1;
    }
    return 0;
  }
  T over(const T& value)const {
    if (value < vmin)
      return  value - vmin;
    else if ( value > vmax )
      return value - vmax;
    return 0;
  }
  void expand(double r, int pivot=0) {
    T new_range = r * (vmax - vmin);
    switch(pivot) {
    case 0: // center
      {
      T sum = (vmin + vmax);
      vmin = (sum - new_range) / 2;
      vmax = (sum + new_range) / 2;
      }
      break;
    case 1: //top
      vmin = vmax - new_range;
      break;
    case -1: //bottom
      vmax = vmin + new_range;
      break;
    }
  }
  void shift(const T& v) {
    vmin += v;
    vmax += v;
  }
  const T& min()const { return vmin; }
  const T& max()const { return vmax; }
  T width()const { return vmax-vmin; }
  T normalize(const T& v) {
    return (v-vmin) / (vmax-vmin);
  }
};


/** フィルターのベースになるもの
 * 入力値と出力値および各1時刻前のデータを持つ
 * 
 * 
 */
template <class T>
class FilterBase {
 public:
  T x;
  T x_bfr;
  T y;
  T y_bfr;
 
  FilterBase(){};
  FilterBase(T x_in) {
    x = x_in;
    x_bfr = x_in;
    y = x_in;
    y_bfr = x_in;
  };    
  void update() {
    x_bfr = x;
    y_bfr = y;
  };
};

/** ローパスフィルタ
 * 
 * 
 * 
 * 
 */
template <class T>
class LPF : public FilterBase<T>{
  double fc = 1.0;//カットオフ周波数
  double ex = exp(-Ts * fc);

 public:
  LPF(){};
  LPF(T x_in) : FilterBase<T>(x_in) {
  };
  LPF(T x_in, double fc_in) : FilterBase<T>(x_in) {
    fc = fc_in;
    ex = exp(-Ts * fc);
  };
  void setCutoff(double fc_in) {
    fc = fc_in;
    ex = exp(-Ts * fc);
  }
  T update(T x_in) {
    this->y = ex * this->y + (1 - ex) * x_in;     // 
    // double Tc = 1.0/(2*PI*fc);
    // this->y=Ts/(Ts+2*Tc)*xm + Ts/(Ts+2*Tc)*this->x - (Ts-2*Tc)/(Ts+2*Tc)*this->y;  
    return this->y;
  };
  // void update(T xm,T& ret_xm);
};

/* ハイパスフィルタ
 * 
 * 
 * 
 * 
 */
template <class T>
class HPF : public FilterBase<T>{
  double fc = 100.0;    //カットオフ周波数
 
 public:
  HPF(){};
  HPF(T x_in, double fc_in) : FilterBase<T>(x_in) {
    fc = fc_in;
  };
  void setCutoff(double fc_in) {
    fc = fc_in;
  };
  T update(T xm) {
    double Tc = 1.0/(2*M_PI*fc);
    this->y = 2.0*Tc/(Ts+2.0*Tc)*xm - 2.0*Tc/(Ts+2.0*Tc)*this->x - (Ts-2.0*Tc)/(Ts+2.0*Tc)*this->y;
    this->x = xm;
    return this->y;
  };
  // void update(T xm,T& ret_xm);
};

// template<class T>
// HPF<T>::HPF(T x_in, double fc_in) : FilterBase<T>(x_in){
//   fc = fc_in;
//   this->y = x_in;
// }

// template <class T>
// T HPF<T>::update(T xm) {
//   double Tc = 1.0/(2*M_PI*fc);
//   //this->x = 2.0*Tc/(Ts+2.0*Tc)*xm - Ts/(Ts+2.0*Tc)*this->x;
//   this->y = 2.0*Tc/(Ts+2.0*Tc)*xm - 2.0*Tc/(Ts+2.0*Tc)*this->x - (Ts-2.0*Tc)/(Ts+2.0*Tc)*this->y;
//   this->x = xm;
//   return this->y;
// }

/* 微分器
 * 
 * 
 * 
 * 
 */
// template <class T>
// class Diff : public FilterBase<T>{
// public:
//   double fc = 1.0;//カットオフ周波数
//   Diff();
//   Diff(T x_in, double fc_in);
//   void update(T xm,T& dxm);
// };

// template<class T>
// Diff<T>::Diff() : FilterBase<T>(){
//   fc = FC_Diff;
// }

// template<class T>
// Diff<T>::Diff(T x_in, double fc_in) : FilterBase<T>(x_in){
//   fc=fc_in;
// }

// template <class T>
// void Diff<T>::update(T xm/*現在値*/,T& dxm/*微分値*/){//@todo:普通にreturnした方がコンパクトにできそう
//   double Tc = 1.0/(2.0*PI*fc);
//   this->x = (xm-this->x_bfr+(Tc-Ts/2.0)*this->x)/(Ts/2.0+Tc);
//   dxm = this->x;
//   this->x_bfr = xm;
// }

}

#endif
