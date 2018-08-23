#ifndef DISCOVER_CONTROLLER_H
#define DISCOVER_CONTROLLER_H

struct StationStruct {
  std::string mcast_addr;
  uint16_t data_port;
  std::string station_name;
}

class DiscoverController {
private:
  int parse_station(struct StationStruct *station_info);
public:
  DiscoverController();

  void run();
  void setup();
}

#endif
