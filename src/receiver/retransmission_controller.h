#ifndef RETRANSMISSION_CONTROLLER_H
#define RETRANSMISSION_CONTROLLER_H

#include "discover_controller.h"


class RetransmissionController {
private:
public:
  RetransmissionController();

  void run();
  void setup();
  void restart(StationInfo station);
};

#endif
