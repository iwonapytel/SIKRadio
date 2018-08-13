#include <iostream>
#include <boost/program_options.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include "sender_parameters_parser.h"
#include "const.h"

namespace po = boost::program_options;

ServerParameters ServerParametersParser::parse(int argc, const char **argv) {
  po::options_description desc("Sender options");
  desc.add_options()
    (",h", "help")
    (",a", po::value<std::string>()->required(), "multicast ip address")
    (",P", po::value<uint16_t>()->default_value(DATA_PORT), "data port")
    (",C", po::value<uint16_t>()->default_value(CTRL_PORT), "control port")
    (",p", po::value<int>()->default_value(PSIZE), "packet size (bytes)")
    (",f", po::value<int>()->default_value(FSIZE), "fifo size (kilobytes)")
    (",R", po::value<int>()->default_value(RTIME), "retransmission time (miliseconds)")
    (",n", po::value<std::string>()->default_value(RECEIVER_NAME), "receiver name")
  ;

  ServerParameters serverParameters;
  try {
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("h")) {
      std::cout << desc << "\n";
      return serverParameters;
    }

    serverParameters.mcast_addr = vm["-a"].as<std::string>();
    serverParameters.data_port = vm["-P"].as<uint16_t>();
    serverParameters.ctrl_port = vm["-C"].as<uint16_t>();
    serverParameters.packet_size = vm["-p"].as<int>();
    serverParameters.fifo_size = vm["-f"].as<int>();
    serverParameters.rtime = vm["-R"].as<int>();
    serverParameters.receiver_name = vm["-n"].as<std::string>();

  } catch (std::exception &err) {
    std::cerr << err.what() << std::endl;
    exit(1);
  }
  return serverParameters;
}
