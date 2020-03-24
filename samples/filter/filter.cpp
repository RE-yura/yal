#include <yal.h>

int main(int argc, char *argv[]) {

  Yal::Vector<3> vec(2.0, 5.0, 4.0);
  Yal::Vector<3> ret;

  Yal::LPF<Yal::Vector<3>> lpf;
  lpf.setCutoff(10.0);

  lpf.x.print();
  lpf.x_bfr.print();
  lpf.y.print();
  lpf.y_bfr.print();
  printf("%f\n", lpf.fc);

  printf("======================\n");

  for(int i=0; i<33; i++) {
    ret = lpf.update(vec);
    ret.print();
  }

  return 0;
}
