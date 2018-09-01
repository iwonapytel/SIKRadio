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
#include <boost/algorithm/string.hpp>
#include <stdexcept>
#include <thread>
#include "main_controller.h"

MainController::MainController(SenderParameters params, SafeSet *safe_set):
  params(params), safe_set(safe_set) {
  std::stringstream ss;
  ss << REPLY << " " << params.mcast_addr << " " << params.data_port
     << " " << params.sender_name;
  reply_msg = ss.str();
  this->setup();
};

void MainController::setup() {
  // Setup listening addr struct
  struct sockaddr_in listening_addr;
  memset(&listening_addr, 0, sizeof(listening_addr));
  listening_addr.sin_family = AF_INET;
  listening_addr.sin_port = htons(params.ctrl_port);
  listening_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  // Setup listening socket
  if ((listening_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    syserr("MainController: create listening socket");

  int optval = 1;
  if (setsockopt(listening_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval))
    syserr("MainController: setsockopt so_reuseaddr");

  if (setsockopt(listening_socket, SOL_SOCKET, SO_BROADCAST, &optval, sizeof optval))
    syserr("MainController: setsockopt so_broadcast");

  int ttl = MAX_TTL;
  if (setsockopt(listening_socket, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof ttl))
    syserr("MainController: setsockopt ip_multicast");

  if (bind(listening_socket, (struct sockaddr*) &listening_addr, sizeof(listening_addr)) < 0)
    syserr("MainController: bind socket");
}

void MainController::run() {
  std::thread([&] {
    struct sockaddr_in receiver_addr{};
    socklen_t socklen = sizeof(receiver_addr);
    char buffer[MAIN_CONTROLLER_BUF];
    int nbytes;

    while (true) {
      memset(&receiver_addr, 0, sizeof(receiver_addr));
      memset(buffer, 0, MAIN_CONTROLLER_BUF);

      if ((nbytes = recvfrom(listening_socket, buffer, sizeof(buffer)-1, 0,
          (struct sockaddr*) &receiver_addr, &socklen)) <= 0)
          syserr("MainController: recvfrom");

      if (!strncmp(buffer, LOOKUP, strlen(LOOKUP))) {
        if (sendto(listening_socket, reply_msg.c_str(),
            reply_msg.size(), 0, (struct sockaddr*) &receiver_addr,
            socklen) < reply_msg.size())
              syslogger("MainController: sendto");
        syslogger("MainController: sending lookup message");
      }

      if (!strncmp(buffer, REXMIT, strlen(REXMIT))) {
          buffer[nbytes] = '\0';
          auto requests = parse_requests(std::string(buffer));
          for (auto &r: requests) {
              safe_set->add(r);
        }
      }
    }
  }).detach();
}

std::vector<uint64_t> MainController::parse_requests(std::string str) {
  std::vector<uint64_t> requests;
  std::vector<std::string> splitted;
  boost::split(splitted, str, boost::is_any_of(", "));

  for (auto it = std::next(splitted.begin()); it != splitted.end(); it++) {
    uint64_t req;
    try {
      req = stoll(*it);
    } catch(std::invalid_argument &e) {
      syslogger("MainController: parse_request invalid argument");
      continue;
    } catch(std::out_of_range &e) {
      syslogger("MainController: parse_request argument ouf of range");
      continue;
    }
    requests.push_back(req);
  }

  return requests;
}
