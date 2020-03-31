#include <yal.h>

int main(int argc, char *argv[]) {

  // Yal::Matrix<1,1> Md = { 1.0 };
  // Yal::Matrix<1,1> Bd = { 100.0 };
  // Yal::Matrix<1,1> Kd = { 5000.0 };
  // Yal::ImpedanceController<Yal::Vector<1>, Yal::Matrix<1,1>> adm(Md,Bd,Kd);

  // Yal::Vector<1> f_human = {1.0 };

  // Yal::Vector<1> dx;
  // Yal::Vector<1> pos;

  // for(int j=0; j<100; j++){
  //   dx = adm.admitance_out(f_human);
  //   // dx.print();
  //   pos += dx;
  //   pos.print();
  // }

  Yal::Matrix<3,3> Md = { 1.0, 0.0, 0.0,
                          0.0, 1.0, 0.0,
                          0.0, 0.0, 1.0 };
  Yal::Matrix<3,3> Bd = { 100.0,   0.0,   0.0,
                            0.0, 100.0,   0.0,
                            0.0,   0.0, 100.0 };
  Yal::Matrix<3,3> Kd = { 5000.0,    0.0,    0.0,
                             0.0, 5000.0,    0.0,
                             0.0,    0.0, 5000.0 };
  Yal::ImpedanceController<Yal::Vector<3>, Yal::Matrix<3,3>> adm(Md,Bd,Kd);

  Yal::Vector<3> f_human = {1.0, 0.0, 0.0};

  Yal::Vector<3> dx;
  Yal::Vector<3> pos;

  for(int j=0; j<100; j++){
    dx = adm.admitance_out(f_human);
    // dx.print();
    pos += dx;
    pos.print();
  }

  
  return 0;
}

