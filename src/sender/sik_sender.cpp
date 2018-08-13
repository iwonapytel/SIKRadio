#include <iostream>
#include <chrono>
#include <boost/program_options.hpp>
#include "sender_parameters_parser.h"

int main(int argc, const char *argv[]) {
    ServerParametersParser Parser;
    ServerParameters server_parameters = Parser.parse(argc, argv);
    // MCAST_ADDR, -a nodefault
    // DATA_PORT, -P default 20730
    // CTRL_PORT -C default 30730
    // PSIZE -p default 512B
    // FSIZE -f defualt 128kB
    // RTIME -R 250
    // NAZWA -n "Nienazwany Nadajnik"
    return 0;
}
