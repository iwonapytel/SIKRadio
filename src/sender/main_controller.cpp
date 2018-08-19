#include <iostream>
#include <err.h>
#include <audio_packet.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <sstream>
#include <const.h>
#include "main_controller.h"

MainController::MainController(SenderParameters params, SafeSet *safe_set):
  params(params), safe_set(safe_set) {
  std::stringstream ss;
  ss << REPLY << " " << params.mcast_addr << " " << params.data_port
     << " " << params.sender_name;
  reply_msg = ss.str();
};

void MainController::setup() {
  // Setup listening addr struct
  sockaddr_in listening_addr;
  memset(&listening_addr, 0, sizeof(listening_addr));
  listening_addr.sin_family = AF_INET;
  listening_addr.sin_port = htons(params.ctrl_port);
  listening_addr.sin_addr = htonl(INADDR_ANY);

  // Setup listening socket
  if (listening_socket = sock(AF_INET, SOCK_DGRAM, 0) < 0)
    syserr("MainController: create listening socket");

  int optval = 1;
  if (setsockopt(listening_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval))
    syserr("MainController: setsockopt so_reuseaddr");

  if (setsockopt(listening_socket, SOL_SOCKET, SO_BROADCAST, &optval, sizeof optval))
    syserr("MainController: setsockopt so_broadcast");

  int ttl = MAX_TTL;
  if (setsockopt(listening_socket, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof ttl));
    syserr("MainController: setsockopt ip_multicast");

  if (bind(listening_socket, &listening_addr, sizeof(listening_addr)));
    syserr("MainController: bind socket")
}

void MainController::run() {
  struct sockaddr_in receiver_addr;
  char buffer[MAIN_CONTROLLER_BUF];
  memset(&receiver_addr, 0, sizeof(receiver_addr));
  memset(&buffer, 0, sizeof(buffer));

  std::thread([&] {
    while (true) {
        if (nbytes = recvfrom(listening_socket, &buffer, sizeof(buffer),
           (struct sockaddr*) &receiver_addr, sizeof(receiver_addr)) < 0)
          syserr("MainController: recvfrom");



    }
  }).detach();
}

std::vector<std::string> parse_request(std::string str) {
  
}
