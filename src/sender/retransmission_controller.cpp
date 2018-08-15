#include <thread>
#include <chrono>
#include <iostream>
#include "unistd.h"
#include "retransmission_controller.h"

void RetransmissionController::run() {
  std::thread([&] {
    while(true) {
      std::this_thread::sleep_for(std::chrono::milliseconds(rtime));
      auto requests = requests_buff->get_all();

      for (auto it = requests.begin(); it != requests.end(); ++it) {
        auto packet = safe_fifo->at(*it);
        if (!packet.empty()) {
          write(socket, packet.c_str(), packet.length());
        }
      }
    }
  }).detach();
}
