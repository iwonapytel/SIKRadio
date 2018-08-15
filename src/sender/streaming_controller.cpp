#include <iostream>


StreamingController::StreamingController(SenderParameters sender_parameters,
                                         SafeFIFO* safe_fifo) {
  this->params = sender_parameters;
  this->safe_fifo = safe_fifo;

  // Setup streaming address
  struct sockaddr_in streaming_addr{};
  memset(&streaming_addr, 0, sizeof(sockaddr_in));

  sockaddr_in.sin_family = AF_INET;
  sockaddr_in.sin_port = htons(params->data_port);
  sockaddr_in

}
