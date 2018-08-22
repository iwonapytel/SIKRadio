#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H

#include <thread_safe_structures.h>
#include "sender_parameters_parser.h"

class MainController {
private:
  SenderParameters params;
  SafeFIFO *safe_fifo;
  SafeSet *safe_set;
  int listening_socket;
  std::string reply_msg;

  std::vector<uint64_t> parse_requests(std::string str);

public:
  MainController(SenderParameters params, SafeSet *safe_set);

  void setup();

  void run();
};

#endif
