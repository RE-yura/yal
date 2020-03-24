#include <yal.h>
#include <string>


int main() {
  Yal::FIFO fifo("my_msg");
  fifo.open(Yal::FIFO::NON_BLOCKING);

  Yal::Vector<3> data;

  while(1) {

    if (fifo.get(&data, sizeof(data)) == sizeof(data)) {
      data.print();
    }
  }

  fifo.destroy();

  return 0;
}
