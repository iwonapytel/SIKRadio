#ifndef SENDER_PARAMETERS_H
#define SENDER_PARAMETERS_H

#include <string>

struct SenderParameters {
  std::string mcast_addr;
  uint16_t data_port;
  uint16_t ctrl_port;
  int packet_size;
  int fifo_size;
  int rtime;
  std::string sender_name;
};

class SenderParametersParser {
public:
  SenderParameters parse(int argc, const char **argv);
  void check_params(SenderParameters params);
};

#endif
