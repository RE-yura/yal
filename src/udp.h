#ifndef __YAL_UDP
#define __YAL_UDP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include "data.h"

namespace Yal {

template <class T, class U>
void* receiver_thread(void* arg);

template <class T>
class UDP {
  int sock;
  struct sockaddr_in addr;

  T data;

 public:
  UDP(std::string address, int port) {
    // ソケットの作成 socket(ドメイン，ソケットタイプ，プロトコル)
    printf("socketを作成します\n");
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(address.c_str());
    addr.sin_port = htons(port);        
  }

  ~UDP() {
    close(sock);
  }

  void bind() {
    ::bind(sock, (const struct sockaddr *)&addr, sizeof(addr));
  }

  void recv(T *rdata) {
    ::recv(sock, rdata, sizeof(*rdata), 0);
    // return data;
  }

  ssize_t send(T *data) {
    return sendto(sock, data, sizeof(*data), 0, (struct sockaddr *)&addr, sizeof(addr));
  }
};

/* Sender class
 * - 指定のIPへ送信
 * - 受信は自動(全てのIPから受け付ける)
 */
template <class T, class U>
class Sender{

  int sock;
  struct sockaddr_in s_addr;
  struct sockaddr_in r_addr;
  pthread_t task_rx;

 public:
  //bool flag_terminate=false;
  
  //const char* server_hostname;
  char server_hostname[64];
  //std::string server_hostname;

  // T sdata;
  U rdata;

  Sender() {
  }

  ~Sender() {
    destroy_connection();
  }

  void create_and_bind_socket(std::string address, int port) {
    // create_socket(address, port);
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    r_addr.sin_family = AF_INET;
    r_addr.sin_addr.s_addr = inet_addr(address.c_str());
    r_addr.sin_port = htons(port);

    bind(sock, (const struct sockaddr *)&r_addr, sizeof(r_addr));
  };

  int create_connection(std::string address, int port) {
    // ソケットの作成 socket(ドメイン，ソケットタイプ，プロトコル)
    printf("socketを作成します\n");
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if( sock < 0 ){
      perror("socket");
      return -1;
    }
    
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = inet_addr(address.c_str());
    s_addr.sin_port = htons(port);

    pthread_create( &task_rx, NULL, receiver_thread<T,U>, this);

    return 1;
  };

  void destroy_connection() {
    std::cout << "socketを破棄" << std::endl;
    close(sock);
    sock = -1;
  };

  ssize_t send(T* buf) {
    if( sock < 0 ) return -1;
    return sendto( sock, buf, sizeof(*buf), 0, (struct sockaddr*)&s_addr, sizeof(s_addr) );
  };
  // ssize_t send(const U& buf,int id);


  void receiver() {
    const size_t size_rx = sizeof(rdata);
    
    struct sockaddr_in from;
    int fromlen = sizeof(from);

    create_and_bind_socket("0.0.0.0", 4001);

    while(1) {
      // // ソケットからの受信メッセージをvbufに格納
      ssize_t rsize = recv(sock, &rdata, size_rx, 0);
      // ssize_t rsize = recvfrom(sock, &rdata, size_rx, 0,//MSG_DONTWAIT, (struct sockaddr*)&from, (socklen_t*)&fromlen);

      printf("recv: [%f, %f, %f]\n", rdata.Pr[0], rdata.Pr[1], rdata.Pr[2]);
      rdata.Pr.print();
    }

    printf("receiver terminated\n");
  };

};

template <class T, class U>
// Sender::receiverを呼び出す
void* receiver_thread(void* arg) {
  Sender<T,U>* sender = (Sender<T,U>*) arg;
  sender->receiver();
  return 0;
}

}

#endif
