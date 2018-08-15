#ifndef RETRANSMISSION_CONTROLLER_H
#define RETRANSMISSION_CONTROLLER_H
#include <thread_safe_structures.h>
#include "sender_parameters_parser.h"

class RetransmissionController {
private:
  SafeSet<uint64_t> *requests_buff;
  SafeFIFO *safe_fifo;
  int rtime;
  int socket;

public:
  RetransmissionController(SafeSet<uint64_t> *requests_buff, SafeFIFO *safe_fifo,
                          SenderParameters parameters, int socket) :
                                                    requests_buff(requests_buff),
                                                    safe_fifo(safe_fifo),
                                                    rtime(parameters.rtime),
                                                    socket(socket) {}

  void run();
};

#endif
