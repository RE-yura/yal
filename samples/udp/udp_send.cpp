#include <stdio.h>
#include <yal.h>

class Data {
 public:
  double pos[3];
};

Yal::UDP<Data> udp0("127.0.0.1",4001);

int main(int argc, char **argv){
  Data sdata;
  sdata.pos[0] = 3.0;
  sdata.pos[1] = 4.0;
  sdata.pos[2] = 5.0;
  udp0.send(&sdata);
  return 0;
}




