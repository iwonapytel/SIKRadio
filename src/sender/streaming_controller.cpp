#include <iostream>
#include <err.h>
#include <audio_packet.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include "streaming_controller.h"


StreamingController::StreamingController(SenderParameters sender_parameters,
                                         SafeFIFO* safe_fifo,
                                         uint64_t session_id) {
  this->params = sender_parameters;
  this->safe_fifo = safe_fifo;
  this->session_id = session_id;
  this->setup();
}

void StreamingController::setup() {
  // Setup streaming address
  struct sockaddr_in streaming_addr{};
  memset(&streaming_addr, 0, sizeof(streaming_addr));

  streaming_addr.sin_family = AF_INET;
  streaming_addr.sin_port = htons(params.data_port);

  if (!IN_MULTICAST(ntohl(inet_addr((params.mcast_addr).c_str()))))
    syserr("Given parameter is not a multicast address");

  if (inet_aton((params.mcast_addr).c_str(), &streaming_addr.sin_addr) == 0)
    syserr("StreamingController: inet_aton");

  // Setup socket
  if ((streaming_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    syserr("StreamingController: creating socket");

  int optval = 1;
  if (setsockopt(streaming_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval))
    syserr("StreamingController: setsockopt so_reuseaddr");

  if (setsockopt(streaming_socket, SOL_SOCKET, SO_BROADCAST, &optval, sizeof optval))
    syserr("StreamingController: setsockopt so_broadcast");

  int ttl = MAX_TTL;
  if (setsockopt(streaming_socket, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof ttl))
    syserr("StreamingController: setsockopt ip_multicast");

  if (connect(streaming_socket, (struct sockaddr*) &streaming_addr, sizeof(streaming_addr)))
    syserr("StreamingController: setsockopt connect");
}

void StreamingController::run() {
    char buffer[params.packet_size];
    bzero(buffer, sizeof(buffer));
    int nbytes;
    struct Packet* audio_packet =
      (struct Packet*) malloc(sizeof(struct Packet) + params.packet_size);
    audio_packet->first_byte_num = htobe64(current_first_byte);

    while (nbytes = read(input_fdes, buffer, params.packet_size)) {
        if (nbytes != params.packet_size)
          continue;

        audio_packet->session_id = htobe64(session_id);
        audio_packet->first_byte_num = htobe64(current_first_byte);

        for (int i = 0; i < PSIZE; i++) {
          audio_packet->audio_data[i] = buffer[i];
        }

        write(streaming_socket, (void *) audio_packet, sizeof(audio_packet));

        const char *packet = reinterpret_cast<char *>(audio_packet);
        std::string str_packet(packet);
        safe_fifo->push(current_first_byte, str_packet);
        current_first_byte += PSIZE;
    }

    if (nbytes < 0)
      syserr("StreamingController: read");

    delete audio_packet;
}
