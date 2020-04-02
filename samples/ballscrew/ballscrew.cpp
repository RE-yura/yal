#include <yal.h>

int main(int argc, char *argv[]) {

  Yal::BallScrew ballscrew(1.0, 0.6);

  print(ballscrew.eta);
  print(ballscrew.calcF(1.0));
  print(ballscrew.calcRps(1.0));

  return 0;
}
