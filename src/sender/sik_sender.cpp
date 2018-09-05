#include <iostream>
#include <ctime>
#include <boost/program_options.hpp>
#include <unistd.h>
#include "sender_parameters_parser.h"
#include "thread_safe_structures.h"
#include "streaming_controller.h"
#include "retransmission_controller.h"
#include "main_controller.h"
#include "primitive_test.h"

int main(int argc, const char *argv[]) {
    SenderParametersParser parser;
    SenderParameters params = parser.parse(argc, argv);
    SafeSet safe_set;
    SafeFIFO safe_fifo(params.fifo_size, params.packet_size);

    //safe_set_test();
    //safe_fifo_test();

    uint64_t session_id = std::time(0);

    MainController
      main_controller(params, &safe_set);
    StreamingController
      streaming_controller(params, &safe_fifo, session_id);
    RetransmissionController
      retransmission_controller(&safe_set, &safe_fifo, params, streaming_controller.streaming_socket);

    main_controller.run();
    retransmission_controller.run();
    streaming_controller.run();


    return 0;
}
