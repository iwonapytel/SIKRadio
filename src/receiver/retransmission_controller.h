#ifndef RETRANSMISSION_CONTROLLER_H
#define RETRANSMISSION_CONTROLLER_H

#include <thread>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include <map>
#include <mutex>
#include <condition_variable>
#include "discover_controller.h"
#include "receiver_parameters_parser.h"

class RetransmissionController {
private:
  uint64_t rtime;
  struct sockaddr_in server_address;
  int retransmission_socket;
  std::map<std::time_t, std::vector<uint64_t>> retransmission_map;
  std::condition_variable condvar;
  std::mutex mutex;
public:
  RetransmissionController(ReceiverParameters params);
  void run();
  void set_address(struct sockaddr_in server_address);
  void setup();
  std::string retransmission_msg(std::vector<uint64_t> packets);
  void restart(StationInfo station);
  void add(uint64_t first, uint64_t last, int psize);
  void remove(uint64_t packet);
};

#endif
