#include "buffer.h"

void Buffer::add(const char* packet, int first_byte) {
  if (id_exists[first] && first_byte < data_id[first])
    return;

  int offset = 0;
  if (id_exists[first])
    offset = first_byte - data_id[first];

  for (int i = 0; i <= packet_size; ++i) {
    int index = (first + offset + i) % size;
    audio_data[index] = packet[i];
    data_id[index] = first_byte + i;
    id_exists[index] = 1;
  }
}

void Buffer::clear(int packet_size) {
  this->first = 0;
  this->packet_size = packet_size;

  memset(audio_data, 0, sizeof(audio_data));
  memset(data_id, 0, sizeof(data_id));
  memset(id_exists, 0, sizeof(id_exists));
}

std::pair<char*, int> Buffer::read() {
  if (!id_exists[first])
    return std::pair<char*, int>(nullptr, 0);

  int read_count = 0;
  for (int i = first; i < size - 1; ++i) {
    if (data_id[i+1] == data_id[i] + 1) {
      read_count++;
    }
  }

  return std::pair<char*, int>(&audio_data[first], read_count);
}

void Buffer::update(int read_count) {
  first += read_count;
  first = first  % size;
}
