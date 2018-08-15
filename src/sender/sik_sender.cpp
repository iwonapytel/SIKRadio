#include <iostream>
#include <chrono>
#include <boost/program_options.hpp>
#include <unistd.h>
#include "sender_parameters_parser.h"
#include "thread_safe_structures.h"
#include "streaming_controller.h"
#include "retransmission_controller.h"
#include "primitive_test.h"
//#include "retransmission_controller.h"

int main(int argc, const char *argv[]) {
    SenderParametersParser parser;
    SenderParameters sender_parameters = parser.parse(argc, argv);
    // MCAST_ADDR, -a nodefault
    // DATA_PORT, -P default 20730
    // CTRL_PORT -C default 30730
    // PSIZE -p default 512B
    // FSIZE -f defualt 128kB
    // RTIME -R 250
    // NAZWA -n "Nienazwany Nadajnik"
    safe_set_test();
    safe_fifo_test();
    return 0;
}
