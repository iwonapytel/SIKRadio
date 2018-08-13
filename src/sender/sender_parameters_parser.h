#ifndef SENDER_PARAMETERS_H
#define SENDER_PARAMETERS_H

#include <string>

struct ServerParameters {
  std::string mcast_addr;
  uint16_t data_port;
  uint16_t ctrl_port;
  int packet_size;
  int fifo_size;
  int rtime;
  std::string receiver_name;
};

class ServerParametersParser {
public:
  ServerParameters parse(int argc, const char **argv);
};

#endif
