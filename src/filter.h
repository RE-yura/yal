//filter and differentiator library
#ifndef __YAL_FILTER
#define __YAL_FILTER

#include<math.h>

#define Fc 1  //カットオフ
#define Ts 0.001  // サンプリング時間

namespace Yal {

/* フィルターのベースになるもの
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

// template <class T>
// FilterBase<T>::FilterBase(){
//   x=0.0;
//   x_bfr=0.0;
//   y = 0.0;
//   y_bfr=0.0;
// }
// template <class T>
// FilterBase<T>::FilterBase(T x_in) {
//   x = x_in;
//   x_bfr = x_in;
//   y = x_in;
//   y_bfr = x_in;
// }

// template<class T>
// void FilterBase<T>::update() {
//   x_bfr = x;
//   y_bfr = y;
// }

/* ローパスフィルタ
 * 
 * 
 * 
 * 
 */
template <class T>
class LPF : public FilterBase<T>{
public:
  double fc = 1.0;//カットオフ周波数
  double ex = exp(-Ts * fc);
  LPF(){};
  LPF(T x_in, double fc_in) : FilterBase<T>(x_in) {
    fc = fc_in;
    ex = exp(-Ts * fc);
  };
  void setCutoff(double fc_in) {
    fc = fc_in;
  }
  T update(T x_in) {
    this->y = ex * this->y + (1 - ex) * x_in;
    return this->y;
  };
  // void update(T xm,T& ret_xm);
};

// template<class T>
// LPF<T>::LPF(T x_in, double fc_in) : FilterBase<T>(x_in){
//   fc = fc_in;
//   ex = exp(-Ts * fc);
// }

// template <class T>
// void LPF<T>::update(T xm, T &ret_xm){
//   double Tc = 1.0/(2*PI*fc);
//   this->y=Ts/(Ts+2*Tc)*xm + Ts/(Ts+2*Tc)*this->x - (Ts-2*Tc)/(Ts+2*Tc)*this->y;  
//   this->x = xm;
//   ret_xm = this->y;
// }


/* ハイパスフィルタ
 * 
 * 
 * 
 * 
 */
template <class T>
class HPF : public FilterBase<T>{
public:
  double fc = 100.0;    //カットオフ周波数
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
