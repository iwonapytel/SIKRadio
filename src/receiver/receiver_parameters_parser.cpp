#include <iostream>
#include <boost/program_options.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <const.h>
#include "receiver_parameters_parser.h"

namespace po = boost::program_options;

ReceiverParameters ReceiverParametersParser::parse(int argc, const char **argv) {
  po::options_description desc("Receiver options");
  desc.add_options()
    (",h", "help")
    (",d", po::value<std::string>()->default_value(DISCOVER_ADDR), "discover station ip address")
    (",C", po::value<uint16_t>()->default_value(CTRL_PORT), "control port")
    (",U", po::value<uint16_t>()->default_value(UI_PORT), "UI port")
    (",b", po::value<int>()->default_value(BSIZE), "buffer size (bytes)")
    (",R", po::value<int>()->default_value(RTIME), "queries for missing packets time")
  ;

  ReceiverParameters receiver_parameters;
  try {
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("h")) {
      std::cout << desc << "\n";
      return receiver_parameters;
    }

    receiver_parameters.discover_addr = vm["-d"].as<std::string>();
    receiver_parameters.ctrl_port = vm["-C"].as<uint16_t>();
    receiver_parameters.ui_port = vm["-U"].as<uint16_t>();
    receiver_parameters.buffer_size = vm["-b"].as<int>();
    receiver_parameters.rtime = vm["-R"].as<int>();

  } catch (std::exception &err) {
    std::cerr << err.what() << std::endl;
    exit(1);
  }
  return receiver_parameters;
}
