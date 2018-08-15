#include <iostream>
#include <boost/program_options.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <const.h>
#include "sender_parameters_parser.h"

namespace po = boost::program_options;

SenderParameters SenderParametersParser::parse(int argc, const char **argv) {
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

  SenderParameters sender_parameters;
  try {
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("h")) {
      std::cout << desc << "\n";
      return sender_parameters;
    }

    sender_parameters.mcast_addr = vm["-a"].as<std::string>();
    sender_parameters.data_port = vm["-P"].as<uint16_t>();
    sender_parameters.ctrl_port = vm["-C"].as<uint16_t>();
    sender_parameters.packet_size = vm["-p"].as<int>();
    sender_parameters.fifo_size = vm["-f"].as<int>();
    sender_parameters.rtime = vm["-R"].as<int>();
    sender_parameters.receiver_name = vm["-n"].as<std::string>();

  } catch (std::exception &err) {
    std::cerr << err.what() << std::endl;
    exit(1);
  }
  return sender_parameters;
}
