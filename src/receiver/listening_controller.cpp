#include <poll.h>
#include <err.h>
#include <const.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include "listening_controller.h"
#include "discover_controller.h"

void ListeningController::run() {
  int p;
  while ((p = poll(sockets, 3, -1)) > 0) {
    remove_timeouts();

    if (sockets[0].revents & POLLIN) {
      syslogger("ListeningController: got REPLY");
      discover_event(sockets[0].fd, sockets[0].revents);
    }

    if (sockets[1].revents & POLLIN) {
      radio_data_event(sockets[1].fd, sockets[1].revents);
    }
  }
  if (p == -1)
    syserr("ListeningController: poll");
}

void ListeningController::restart(Restart restart) {
  syslogger("ListeningController: Restart");
  status = Status::OFF;
  clean_session();
  sockets[2].events = 0;
  drop_connection();
  StationInfo new_station = StationInfo();
  new_station.valid = false;

  switch(restart) {
    case Restart::DEFAULT: {
      auto it = stations.begin();
      for (; it != stations.end(); it++) {
        if (it->station_name == params.default_station_name) {
          new_station = (*it);
        }
      }
      break;
    }
    case Restart::FIRST: {
      if (!stations.empty()) {
        new_station = stations[0];
      }
      break;
    }
  }

  if (new_station.valid) {
    sockets[1].fd = create_connection(new_station);
    sockets[1].events = POLLIN;
    status = Status::INITIALIZED;
    session_info.station = new_station;
    retransmission_ctrl.restart(new_station);
  }
}

void ListeningController::remove_timeouts() {
  auto now = std::time(nullptr);
  syslogger("ListeningController: remove timeouts");
  int size = stations.size();
  printf("stations: %d\n", size);
  for (auto it = stations.begin(); it != stations.end();) {
    if ((now - it->last_info) > LOOKUP_TIMEOUT) {
      syslogger("ListeningController: removing timeout");
      auto to_erase = (*it);
      it = stations.erase(it);
      if (to_erase == session_info.station) {
        restart(Restart::FIRST);
      }

    } else {
      it++;
    }
  }
}

void ListeningController::discover_event(int socket, uint16_t event) {
  char buffer[REPLY_MSG_MAX];
  memset(&buffer, 0, sizeof(buffer));
  sockaddr_in server_addr;
  socklen_t socklen = sizeof(server_addr);

  if (recvfrom(socket, buffer, sizeof(buffer), 0, (struct sockaddr*) &server_addr,
    &socklen) < 0) {
    syslogger("ListeningController: discover_event recvfrom");
  }

  server_addr.sin_port = htons(params.ctrl_port);
  std::string reply(buffer);
  update_stations(reply, server_addr);

  if (status == Status::OFF && stations.size() > 0) {
    restart(Restart::FIRST);
  }
}

void ListeningController::update_stations(std::string buffer, sockaddr_in address) {
  StationInfo station = this->discover_ctrl.parse_reply(buffer);
  if (station.valid) {
    auto now = std::time(nullptr);
    auto it = stations.begin();
    station.address = address;

    for (;it != stations.end(); it++) {
      if (*it == station) {
        syslogger("Updating time");
        station.last_info = now;
        break;
      }
    }

    if (it == stations.end()) {
      stations.push_back(station);
      if (station.station_name == params.default_station_name)
        restart(Restart::DEFAULT);
    }
  }
}

void ListeningController::radio_data_event(int socket, uint16_t event) {

}

void ListeningController::setup() {
  // sockets[0] = discover socket
  // sockets[1] = currently played radio
  // sockets[2] = stdout
  fcntl(STDOUT_FD, F_SETFL, O_NONBLOCK);

  sockets[0].fd = discover_socket;
  sockets[0].events = POLLIN;
  sockets[0].revents = 0;

  sockets[1].fd = -1;
  sockets[1].events = POLLIN;
  sockets[1].revents = 0;

  sockets[2].fd = STDOUT_FD;
  sockets[2].events = 0;
  sockets[2].revents = 0;
}

void ListeningController::clean_session() {
  session_info.session_id = 0;
  session_info.first_byte = 0;
  session_info.last_packet = 0;
  session_info.packet_size = 0;
  session_info.station.valid = false;
}

void ListeningController::drop_connection() {
  if (listening_socket == -1) {
    return;
  }
  if (setsockopt(listening_socket, IPPROTO_IP, IP_DROP_MEMBERSHIP, (void*) &mreq, sizeof(mreq)))
    syserr("ListeningController: drop connection");
  close(listening_socket);
}

int ListeningController::create_connection(StationInfo station_info) {
  // Setup socket
  if ((listening_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    syserr("ListeningController: creating socket");

  mreq.imr_interface.s_addr = htonl(INADDR_ANY);
  if (!inet_aton(station_info.mcast_addr.c_str(), (struct in_addr*) &mreq.imr_multiaddr.s_addr))
    syserr("ListeningController: create_connection inet_aton");

  int optval = 1;

  if (setsockopt(listening_socket, SOL_SOCKET, SO_REUSEADDR, (void*) &optval, sizeof(optval)))
    syserr("ListeningController: create_connection setsockopt so_reuseaddr");

  if (setsockopt(listening_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*) &mreq, sizeof(mreq)))
    syserr("ListeningController: create_connection setsockopt ip_add_membership");

  // Setup address
  struct sockaddr_in listening_addr;
  memset(&listening_addr, 0, sizeof(listening_addr));

  listening_addr.sin_family = AF_INET;
  listening_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  listening_addr.sin_port = htons(station_info.data_port);

  //bind
  if (bind(listening_socket, (struct sockaddr*) &listening_addr, sizeof(listening_addr)) < 0)
    syserr("ListeningController: create_connection bind");

  return listening_socket;
}

ListeningController::ListeningController(ReceiverParameters params, DiscoverController &discover_ctrl,
  RetransmissionController &retransmission_ctrl) : params(params), discover_ctrl(discover_ctrl),
  retransmission_ctrl(retransmission_ctrl) {
    discover_socket = this->discover_ctrl.discover_socket;
    listening_socket = -1;
    session_info = SessionInfo();
    status = Status::OFF;
    this->setup();
}
