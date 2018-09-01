#ifndef LISTENING_CONTROLLER_H
#define LISTENING_CONTROLLER_H

#include <poll.h>
#include <netinet/in.h>
#include "discover_controller.h"
#include "retransmission_controller.h"

enum Restart {
  FIRST,
  DEFAULT
};

enum Status {
  OFF,
  ON,
  INITIALIZED
};

class SessionInfo {
public:
  uint64_t session_id;
  uint64_t first_byte;
  uint64_t last_packet;
  int packet_size;
  StationInfo station;

  SessionInfo() {
    session_id = 0;
    first_byte = 0;
    last_packet = 0;
    packet_size = 0;
    station = StationInfo();
    station.valid = false;
  }
};

class ListeningController {
private:
  int discover_socket;
  int listening_socket;
  DiscoverController &discover_ctrl;
  RetransmissionController &retransmission_ctrl;
  struct pollfd sockets[3];
  struct ip_mreq mreq; // Struct used to become a member of multicast group.
  SessionInfo session_info;
  ReceiverParameters params;
  std::vector<StationInfo> stations;
  Status status;
public:
  ListeningController(ReceiverParameters params, DiscoverController &discover_ctrl,
    RetransmissionController &retransmission_ctrl);
  void run();
  void setup();
  void clean_session();
  void update_stations(std::string buffer, sockaddr_in sockaddr);
  void discover_event(int socket, uint16_t event);
  void radio_data_event(int socket, uint16_t event);
  void drop_connection();
  int  create_connection(StationInfo station_info);
  void remove_timeouts();
  void restart(Restart restart);

};

#endif
