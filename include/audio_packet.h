#ifndef AUDIO_PACKET_H
#define AUDIO_PACKET_H

// Zero-length array trick, as in gcc.gnu.org/onlinedocs/gcc/Zero-Length.html
struct Packet {
  uint64_t session_id;
  uint64_t first_byte_num;
  char audio_data[0];
};

#endif
