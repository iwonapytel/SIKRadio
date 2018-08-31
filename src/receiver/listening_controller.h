#ifndef LISTENING_CONTROLLER_H
#define LISTENING_CONTROLLER_H

#include <netinet/in.h>
#include "discover_controller.h"
#include "retransmission_controller.h"

struct SessionInfo {
  uint64_t session_id;
  uint64_t first_byte;
  uint64_t last_packet;
  int psize;
  StationInfo station;

  void clean() {
  }
};

class ListeningController {
private:
  int discover_socket;
  DiscoverController discover_ctrl;
  struct pollfd sockets[3];
  struct ip_mreq mreq; // Struct used to become a member of multicast group.
  std::vector<StationInfo> stations;
public:
  ListeningController(ReceiverParameters params, DiscoverController discover_ctrl,
    RetransmissionController &retransmissionController);
  void run();
  void setup();
};

#endif
