#ifndef RECEIVER_BUFFER_H
#define RECEIVER_BUFFER_H

class Buffer {
private:
  int packet_size;
  int size;
  int first = 0;
  char *audio_data;
  uint64_t *data_id;
  bool *id_exists;

public:
  // Moved constructor and destructor to header file, due to some linker errors(?)
  Buffer(int size): size(size) {
    audio_data = new char[size];
    data_id = new uint64_t[size];
    id_exists = new bool[size];
    memset(audio_data, 0, sizeof(audio_data));
    memset(data_id, 0, sizeof(data_id));
    memset(id_exists, 0, sizeof(id_exists));
  }

  ~Buffer() {
    delete audio_data;
    delete data_id;
    delete id_exists;
  }

  void add(const char* packet, int first_byte);
  void clean(int packet_size);
  void read();
  void update();
};

#endif
