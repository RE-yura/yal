#ifndef __YAL_BALLSCREW
#define __YAL_BALLSCREW

#include<math.h>

namespace Yal {

class BallScrew {
public:
  double lead;  // リード(mm)
  double tau;   // トルク(N･mm)
  double F;     // 推力(N)

  double eta;   // 正効率(0.7~1.0)
  double mu1 = 0.004;       // 正摩擦係数
  double mu2 = 0.008;       // 逆摩擦係数
  double theta = M_PI/4.0;  // ボールねじ溝接触角
  double bcd;               // ボール中心径(mm)

  double v;     // 並進速度
  double rps;   // ボールねじの回転速度
  
  BallScrew(){};

  // BallScrew(リード[mm]，ボール中心径[mm])
  BallScrew(double lead_in, double bcd_in) {
    lead = lead_in;
    bcd = bcd_in;
    double tanB = lead / (M_PI*bcd);
    double tanRho1 = mu1 / cos(theta);
    double tanRho2 = mu2 / cos(theta);
    eta = (1-tanB*tanRho1) / (1+tanRho1/tanB);
  };
  
  ~BallScrew(){};

  double calcTau(double F) {
    tau = F*lead / (2*M_PI*eta);
    return tau;
  }
  double calcF(double tau) {
    F = 2*M_PI*eta*tau / lead;
    return F;
  }

  // 引数は並進速度(m/s)
  double calcRps(double v) {
    rps = v / (lead/1e+3);  // リードはmに直す
    return rps;
  }
};

}

#endif
