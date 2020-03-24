#include <yal.h>
#include <string>
#include <unistd.h>

int main() {
  Yal::FIFO fifo("my_msg");
  fifo.open(Yal::FIFO::NON_BLOCKING);

  Yal::Vector<3> data(2.0, 3.0, 4.0);

  while(1) {
    fifo.ovrwr_put(&data, sizeof(data));
    sleep(1);
  }

  fifo.destroy();

  return 0;
}
