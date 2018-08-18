#include <iostream>
#include <err.h>
#include <audio_packet.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include "main_controller.h"

MainController::MainController(SenderParameters params, SafeSet *safe_set):
  params(params), safe_set(safe_set);

void MainController::run() {

}

void MainController::setup() {

}
