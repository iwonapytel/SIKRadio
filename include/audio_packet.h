#ifndef AUDIO_PACKET_H
#define AUDIO_PACKET_H

struct Packet {
  uint64_t session_id;
  uint64_t first_byte_num;
  byte[] audio_data;
}

#endif
