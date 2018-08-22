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
    SenderParameters params = parser.parse(argc, argv);
    SafeSet safe_set();
    SafeFIFO safe_fifo(params.fifo_size, params.packet_size);
    // MCAST_ADDR, -a nodefault
    // DATA_PORT, -P default 20730
    // CTRL_PORT -C default 30730
    // PSIZE -p default 512B
    // FSIZE -f defualt 128kB
    // RTIME -R 250
    // NAZWA -n "Nienazwany Nadajnik"
    safe_set_test();
    safe_fifo_test();
    //MainController main_controller(params, &safe_set).run();
    //RetransmissionController retransmission_controller(&)
    //StreamingController streaming_controller()


    return 0;
}
