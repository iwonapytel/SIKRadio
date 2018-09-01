#ifndef DISCOVER_CONTROLLER_H
#define DISCOVER_CONTROLLER_H

#include <iostream>
#include <unistd.h>
#include <ctime>
#include <vector>
#include <tuple>
#include <netinet/in.h>
#include "receiver_parameters_parser.h"


class StationInfo {
public:
  std::string mcast_addr;
  uint16_t data_port;
  std::string station_name;
  std::time_t last_info;
  bool valid = true;
  struct sockaddr_in address;

  StationInfo(std::string mcast_addr, uint16_t data_port, std::string station_name,
      std::time_t last_info):
    mcast_addr(mcast_addr),
    data_port(data_port),
    station_name(station_name),
    last_info(last_info) {};

  StationInfo() {};

  friend bool operator==(const StationInfo& x, const StationInfo& y) {
    return std::tie(x.mcast_addr, x.data_port, x.station_name)
      == std::tie(y.mcast_addr, y.data_port, y.station_name);
  }
};

class DiscoverController {
private:
  ReceiverParameters params;
  struct sockaddr_in discover_sockaddr;
  //int parse_station(struct StationStruct *station_info);
public:
  int discover_socket;
  DiscoverController(ReceiverParameters params);

  void run();
  void setup();
  StationInfo parse_reply(std::string msg);
};

#endif
