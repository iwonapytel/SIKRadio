#include <err.h>
#include <thread>
#include <chrono>
#include <stdlib.h>
#include <boost/algorithm/string.hpp>
#include <stdexcept>
#include <const.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "discover_controller.h"

DiscoverController::DiscoverController(ReceiverParameters params):
  params(params) {
    this->setup();
  }

void DiscoverController::run() {
  std::thread([&] {
    while (true) {
      int msg_len = sizeof(LOOKUP);
      if (sendto(discover_socket, LOOKUP, msg_len, 0, (struct sockaddr*) &discover_sockaddr,
          sizeof(discover_sockaddr)) < msg_len)
        syserr("DiscoverController: sendto");

      syslogger("DiscoverController: LOOKUP");
      std::this_thread::sleep_for(std::chrono::milliseconds(DISCOVER_TIME));
    }
  }).detach();
}

void DiscoverController::setup() {
  // Setup addr struct
  memset(&discover_sockaddr, 0, sizeof(struct sockaddr_in));

  discover_sockaddr.sin_family = AF_INET;
  discover_sockaddr.sin_port = htons(params.ctrl_port);

  if (inet_aton((params.discover_addr).c_str(), &discover_sockaddr.sin_addr) == 0)
    syserr("DiscoverController: inet aton");

  // Setup socket
  int optval = 1;

  if ((discover_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    syserr("DiscoverController: discover socker");

  if (setsockopt(discover_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval))
    syserr("DiscoverController: setsockopt so_reuseaddr");

  if (setsockopt(discover_socket, SOL_SOCKET, SO_BROADCAST, &optval, sizeof optval))
    syserr("DiscoverController: setsockopt so_broadcast");
}

StationInfo DiscoverController::parse_reply(std::string str) {
  StationInfo station;
  station.valid = false;
  struct in_addr addr;

  std::vector<std::string> splitted;
  boost::split(splitted, str, boost::is_any_of(" "));
  if (splitted.size() < 4) {
    syslogger("DiscoverController: error in parse, invalid format of reply");
    return station;
  }

  if (inet_aton(splitted[1].c_str(), &addr) == 0) {
    syslogger("DiscoverController: error in parse, invalid mcast address");
    return station;
  }

  int ctrl_port;
  try {
    ctrl_port = std::stoi(splitted[2]);
  } catch (...) {
    syslogger("DiscoverController: error in parse, invalid ctrl port");
    return station;
  }

  if (ctrl_port < 1 || ctrl_port > MAX_SHORT)
    return station;

  std::string name = "";
  for (int i = 3; i != splitted.size(); ++i) {
    name += splitted[i];
  }

  station.valid = true;
  station.mcast_addr = splitted[1];
  station.data_port = static_cast<uint16_t>(ctrl_port);
  station.station_name = name;
  station.last_info = std::time(nullptr);
  return station;
}
