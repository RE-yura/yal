#include <stdio.h>
#include <string>
#include <yal.h>

class Data {
 public:
  double pos[3];
};

Yal::UDP<Data> udp0("0.0.0.0",4001);

int main(int argc, char **argv){
  udp0.bind();
  while (1) {
    Data rdata;
    udp0.recv(&rdata);
    printf("recv: [%f, %f, %f]\n", rdata.pos[0], rdata.pos[1], rdata.pos[2]);
  }
  return 0;
}
