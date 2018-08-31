#include <err.h>
#include "listening_controller.h"

ListeningController::ListeningController(ReceiverParameters params, DiscoverController discover_ctrl,
  RetransmissionController &retransmissionController) {}

void ListeningController::run() {
  while ((rv = poll(sockets, 3, -1)) > 0) {
    update_stations()

    if (sockets[0].revents & POLLIN) {
      lookup_event();
    }
    if (sockets[1].revents & POLLIN) {
      radio_event();
    }
  }
  if (rv == -1)
    syserr("ListeningController: poll")
}

void update_stations() {

}

void ListeningController::setup() {
  // sockets[0] = lookup socket
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

void ListeningController::drop_connection() {
  if (setsockopt(listening_socket, IPPROTO_IP, IP_DROP_MEMBERSHIP, (void*) &mreq, sizeof(mreq)))
    syserr("ListeningController: drop connection");
  close(listening_socket);
}

void ListeningController::create_connection(StationInfo station_info) {
  // Setup socket
  if ((listening_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    syserr("ListeningController: creating socket");

  mreq.imr_interface.s_addr = htonl(INADDR_ANY);
  if (!inet_aton(station_info.mcast_addr.c_str(), mreq.imr_multiaddr.s_addr))
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
  listening_addr.sin_port = htons(station_info.ctrl_port);

  //bind
  if (bind(listening_socket, (struct sockaddr*) &listening_addr, sizeof(listening_addr)) < 0)
    syserr("ListeningController: create_connection bind");
}
