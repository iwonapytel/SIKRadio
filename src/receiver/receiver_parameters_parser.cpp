#include <iostream>
#include <boost/program_options.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <const.h>
#include <err.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include "receiver_parameters_parser.h"


namespace po = boost::program_options;

bool check_is_broadcast_addr(std::string discover_addr) {
  if (!strcmp(DISCOVER_ADDR, discover_addr.c_str()))
    return true;

  struct ifaddrs *ifa, *ifaddr;
  bool is_broadcast = false;
  if (getifaddrs(&ifaddr) < 0)
    syslogger("getifaddr");

  for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_flags & IFF_BROADCAST &&
        ifa->ifa_broadaddr != NULL &&
        ifa->ifa_broadaddr->sa_family == AF_INET) {

      struct sockaddr_in *sin = (struct sockaddr_in *) ifa->ifa_broadaddr;
      const char* ip = inet_ntoa(sin->sin_addr);
      //fprintf(stderr, "%s\n", ip);
      if (!strcmp(ip, discover_addr.c_str()))
        is_broadcast = true;
        break;
    }
  }

  freeifaddrs(ifaddr);
  return is_broadcast;
}

void ReceiverParametersParser::check_params(ReceiverParameters params) {
  if (params.ui_port <= 1024 || params.ctrl_port <= 1024)
    syserr("Port numbers < 1024 restricted to the root user only");
  if (params.ui_port == params.ctrl_port)
    syserr("UI port and control port need to be different");
  if (params.buffer_size <= 0)
    syserr("Buffer size argument require positive value");
  if (params.rtime <= 0)
    syserr("Rtime argument require positive value");

  if(!check_is_broadcast_addr(params.discover_addr))
    syserr("Argument is not a valid broadcast address");
  //if (params.default_station_name.size() <= 0 || params.sender_name.size() > 64)
  //  syserr("Sender name require length between 1 and 64");
}

ReceiverParameters ReceiverParametersParser::parse(int argc, const char **argv) {
  po::options_description desc("Receiver options");
  desc.add_options()
    (",h", "help")
    (",d", po::value<std::string>()->default_value(DISCOVER_ADDR), "discover station ip address")
    (",C", po::value<uint16_t>()->default_value(CTRL_PORT), "control port")
    (",U", po::value<uint16_t>()->default_value(UI_PORT), "UI port")
    (",b", po::value<int>()->default_value(BSIZE), "buffer size (bytes)")
    (",R", po::value<int>()->default_value(RTIME), "queries for missing packets time")
    (",n", po::value<std::string>(), "default station name")
  ;

  ReceiverParameters receiver_parameters;
  try {
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("-h")) {
      std::cout << desc << "\n";
      return receiver_parameters;
    }

    std::string default_name = "";

    if (vm.count("-n")) {
      auto name = vm["-n"].as<std::string>();
      if (name.size() <= 0 || name.size() > 64)
        syserr("Station name require length between 1 and 64");
    }

    receiver_parameters.default_station_name = default_name;

    receiver_parameters.discover_addr = vm["-d"].as<std::string>();
    receiver_parameters.ctrl_port = vm["-C"].as<uint16_t>();
    receiver_parameters.ui_port = vm["-U"].as<uint16_t>();
    receiver_parameters.buffer_size = vm["-b"].as<int>();
    receiver_parameters.rtime = vm["-R"].as<int>();

  } catch (std::exception &err) {
    std::cerr << err.what() << std::endl;
    exit(1);
  }

  check_params(receiver_parameters);
  return receiver_parameters;
}
