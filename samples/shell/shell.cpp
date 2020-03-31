#include <yal.h>
// #include "../../src/yal.h"
// #include "../../src/print.h"

int main(int argc, char *argv[]) {
  Yal::shell("ls");

  double freq = Yal::get_cpufreq();
  print(freq);
}
