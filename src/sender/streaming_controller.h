#ifndef STREAMING_CONTROLLER_H
#define STREAMING_CONTROLLER_H

#include "sender_parameters_parser.h"
#include <thread_safe_structures.h>
#include <const.h>

class StreamingController {
private:
  SenderParameters params;
  int current_packet_id = 0;
  int session_id;
  int streaming_socket;
  int SafeFIFO *safe_fifo;

public:
  StreamingController(SenderParameters senderParameters, SafeFIFO *safe_fifo);
  void start();
};

#endif
