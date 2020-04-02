//インピーダンス制御用
//@todo:インピーダンス型の実装
//未チェック
#ifndef __YAL_IMPEDANCE_CONTROL
#define __YAL_IMPEDANCE_CONTROL

#include "la.h"

#ifndef Ts
#define Ts 0.001  // サンプリング時間
#endif

namespace Yal{

template<class T, class Tmat>
class ImpedanceController {

public:
  // RobotData<T> zero;//基準位置   
  //desired imoedance
  Tmat M;  //N/m/s^2=kg
  Tmat B;  //N/m/s
  Tmat K;  //N/m
  //admitance control
  T v;
  T v_b1;
  T v_b2;
  T fext;
  T fext_b1;
  T fext_b2;
  
  ImpedanceController();
  ImpedanceController(Tmat M_in, Tmat B_in, Tmat K_in);
  void set_impedance(Tmat M_in, Tmat B_in, Tmat K_in);
  T impedance_out(T v_in);      //運動入力力出力  
  T admitance_out(T fext_in);    //力入力運動出力
};

template<class T,class Tmat>
ImpedanceController<T,Tmat>::ImpedanceController(){
}

template<class T,class Tmat>
ImpedanceController<T,Tmat>::ImpedanceController(Tmat M_in, Tmat B_in, Tmat K_in){
  M = M_in;
  B = B_in;
  K = K_in;
}

template<class T,class Tmat>
void ImpedanceController<T,Tmat>::set_impedance(Tmat M_in, Tmat B_in, Tmat K_in){
  M = M_in;
  B = B_in;
  K = K_in;
}

template<class T,class Tmat>
T ImpedanceController<T,Tmat>::impedance_out(T v_in){
  T f_ret = M * v;
  return f_ret; 
  // f = Md * state.d2x + Cd * state.dx + Kd * state.x;//@todo:動力学補償入れる
}

template<class T,class Tmat>
T ImpedanceController<T,Tmat>::admitance_out(T fext_in){//速度出力
  // this->fext = fext;
  // this->ref.dx = 1.0/(4*Md+2*Ts*Cd+Kd*Ts*Ts)*(2*Ts*(fext-this->fext_b2)-(2*Kd*Ts*Ts-8*Md)*this->ref_b1.dx-(4*Md-2*Ts*Cd+Kd*Ts*Ts)*this->ref_b2.dx);
  
  // this->fext_b2 = this->fext_b1;
  // this->fext_b1 = fext;
  // this->ref_b2 = this->ref_b1;
  // this->ref_b1 = this->ref;
  // ref.dx = this->ref.dx*1000.0;//mm/sに直す

  T v_ret;
  fext = fext_in;
  // printf("1 ");
  // ( 2*Ts*(fext-fext_b2) ).print();
  // printf("2 ");
  // ( (-2*K*pow(Ts,2)+8*M)*v_b1 ).print();
  // printf("3 ");
  // ( (2*B*Ts-K*pow(Ts,2)-4*M)*v_b2 ).print();
  v_ret = (2*B*Ts+K*pow(Ts,2)+4*M).inv() * ( 2*Ts*(fext-fext_b2) + (-2*K*pow(Ts,2)+8*M)*v_b1 + (2*B*Ts-K*pow(Ts,2)-4*M)*v_b2 );

  fext_b2 = fext_b1;
  fext_b1 = fext;
  v_b2 = v_b1;
  v_b1 = v_ret;

  return v_ret;
}

}


#endif
