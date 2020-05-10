#include <yal.h>

int main(int argc, char *argv[]) {
  print(Yal::formatTime(3.2, 0));

  double time;

  Yal::Timer timer;
  timer.reset();
  sleep(1);
  time = timer.get_time();
  print(time);

  Yal::GlobalTimer::reset();
  sleep(1);
  time = Yal::GlobalTimer::get_time();
  print(time);
}
