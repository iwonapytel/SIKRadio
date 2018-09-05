#include <iostream>
#include <chrono>
#include "discover_controller.h"
#include "retransmission_controller.h"
#include "receiver_parameters_parser.h"
#include "listening_controller.h"
#include "buffer.h"

int main(int argc, const char *argv[]) {
  ReceiverParametersParser parser;
  ReceiverParameters params = parser.parse(argc, argv);
  DiscoverController discover_controller(params);
  RetransmissionController retransmission_controller(params);
  ListeningController listening_controller(params, discover_controller,
    retransmission_controller);

  retransmission_controller.run();
  discover_controller.run();
  listening_controller.run();
  return 0;
}
