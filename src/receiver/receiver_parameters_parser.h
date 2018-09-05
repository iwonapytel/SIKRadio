#ifndef RECEIVER_PARAMETERS_H
#define RECEIVER_PARAMETERS_H

#include <string>

struct ReceiverParameters {
  std::string discover_addr; //-d
  uint16_t ctrl_port; //-C
  uint16_t ui_port; //-U
  int buffer_size; //-b
  int rtime; //-R
  std::string default_station_name; //-n
};

class ReceiverParametersParser {
public:
  ReceiverParameters parse(int argc, const char **argv);
  void check_params(ReceiverParameters params);
};

#endif
