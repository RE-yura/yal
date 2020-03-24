#include <stdio.h>
#include <yal.h>

Yal::Sender<Yal::BufMS, Yal::BufSM> sender;
Yal::BufMS buf_ms;

int count;

int main(int argc, char **argv){
  sender.create_connection("127.0.0.1", 4001);   // localhost
  buf_ms.Pr[0] = 0.0;
  printf("print1\n");

  while (1) {
    printf("while%d\n", count);
    sender.send(&buf_ms);
    buf_ms.Pr[0] += 2.0;
    buf_ms.Pr[1] = 2.0;
    buf_ms.Pr[2] = 3.0;
    usleep(500000);
    count++;
  }

  return 0;
}
