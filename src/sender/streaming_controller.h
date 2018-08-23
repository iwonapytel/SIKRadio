#ifndef STREAMING_CONTROLLER_H
#define STREAMING_CONTROLLER_H

#include "sender_parameters_parser.h"
#include <thread_safe_structures.h>
#include <const.h>

class StreamingController {
private:
  SenderParameters params;
  uint64_t current_first_byte = 0;
  uint64_t session_id;
  int input_fdes = STDIN_FD;
  SafeFIFO *safe_fifo;

public:
  int streaming_socket;
  StreamingController(SenderParameters sender_parameters, SafeFIFO *safe_fifo,
    uint64_t session_id);
  void run();
  void setup();
};

#endif
