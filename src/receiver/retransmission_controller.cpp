#include <algorithm>
#include <iostream>
#include <const.h>
#include <err.h>
#include <sstream>
#include <chrono>
#include "retransmission_controller.h"

using namespace std::chrono_literals;

RetransmissionController::RetransmissionController(ReceiverParameters params) {
  this->rtime = params.rtime;
  this->setup();
}

void RetransmissionController::setup() {
  timeval timeout;
  timeout.tv_sec = 1;
  timeout.tv_usec = 0;

  if ((retransmission_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    syserr("RetransmissionController: create retransmission_socket");

  if (setsockopt(retransmission_socket, SOL_SOCKET, SO_SNDTIMEO, (void*) &timeout,
      sizeof(timeout)) < 0)
    syserr("RetransmissionController: set timeout option");
}

void RetransmissionController::run() {
  std::thread([&] {
    while (true) {
      auto now = std::chrono::system_clock::now();
      now += rtime*1ms;

      std::unique_lock<std::mutex> lock(mutex);
      condvar.wait_until(lock, now, [&](){ return !retransmission_map.empty(); });

      uint64_t next_time = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
      while (!retransmission_map.empty() && retransmission_map.begin()->first <= next_time) {
          auto elem = retransmission_map.begin();
          auto next_time = std::time(nullptr) + rtime;
          auto packets = elem->second;
          retransmission_map.erase(elem);

          auto retry_msg = retransmission_msg(packets);

          auto since_epoch = std::chrono::system_clock::now().time_since_epoch();
          uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(since_epoch).count();
          next_time = now + rtime;
          if (sendto(retransmission_socket, retry_msg.c_str(), retry_msg.size(),
              0, (const sockaddr*) &server_address, sizeof(server_address)) < 0)
              syslogger("RetransmissionController: sendto");

          retransmission_map[next_time] = packets;
      }
    }
  }).detach();
}

std::string RetransmissionController::retransmission_msg(std::vector<uint64_t> packets) {
  std::stringstream ss;
  ss << REXMIT << " ";

  size_t length = packets.size();
  for (auto i = 0; i < length; ++i) {
    ss << packets[i];
    if (i != length - 1) {
      ss << ",";
    }
  }

  std::string retry_msg = ss.str();
}

void RetransmissionController::add(uint64_t first, uint64_t last, int psize) {
  std::unique_lock<std::mutex> lock(mutex);
  std::vector<uint64_t> packets;

  for (auto i = first; i < last; i += psize) {
    packets.push_back(i);
  }
  auto since_epoch = std::chrono::system_clock::now().time_since_epoch();
  uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(since_epoch).count();

  auto first_retr = now + rtime;
  retransmission_map[first_retr] = packets;

  condvar.notify_one();
}

void RetransmissionController::remove(uint64_t packet_id) {
  std::unique_lock<std::mutex> lock(mutex);
  for (auto &i : retransmission_map) {
    auto packets = i.second;
    auto packet = std::find(packets.begin(), packets.end(), packet_id);
    if ( packet != packets.end()) {
        packets.erase(packet);

        if (packets.empty())
          retransmission_map.erase(i.first);
        break;
      }
  }
}

void RetransmissionController::restart(StationInfo station) {
  std::unique_lock<std::mutex> lock(mutex);
  retransmission_map.clear();
  server_address = station.address;
  condvar.notify_one();
}
