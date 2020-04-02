#include <yal.h>

int main(int argc, char *argv[]) {

  Yal::PPTrajectory<double> ptp;

  double t = 0.0;
  bool flag_ptp = false;

  double q = 0.0;
  double qref;

  ptp.create(1.0, q, 0.1, t);

  for (int j=0; j<40; j++) {
    qref = ptp.get(t, &flag_ptp);
    q = qref;
    print(flag_ptp, qref);

    t += 0.001;
  }

  ptp.modify_destination(-1.0, q); 
  printf("================\n");
  
  for (int j=0; j<40; j++) {
    qref = ptp.get(t, &flag_ptp);
    q = qref;
    print(flag_ptp, qref);

    t += 0.001;
  }

  ptp.modify_destination(-2.0, q);
  printf("================\n");
  

  for (int j=0; j<40; j++) {
    q = qref;
    qref = ptp.get(t, &flag_ptp);
    print(flag_ptp, qref);

    t += 0.001;
  }

  return 0;

  
}
