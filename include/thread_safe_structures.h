#ifndef THREAD_SAFE_STRUCTURES_H
#define THREAD_SAFE_STRUCTURES_H

#include <vector>
#include <map>
#include <set>
#include <mutex>

template<typename T>
class SafeSet {
private:
  std::mutex mutex;
  std::set<T> set;
public:
  SafeSet() {};

  void add(T element) {
    std::unique_lock<std::mutex> lock(this->mutex);
    set.insert(element);
  }

  std::vector<T> get_all() {
    std::unique_lock<std::mutex> lock(this->mutex);
    std::vector<T> vec;
    std::copy(set.begin(), set.end(), std::back_inserter(vec));
    set.clear();
    return vec;
  }
};

class SafeFIFO {
private:
  std::mutex mutex;
  int buffer_size;
  std::map<uint64_t, std::string> map;

public:
  SafeFIFO(int size, int psize) {
    this->buffer_size = size * psize;
  };

  SafeFIFO() {};

  void push(uint64_t packet_no, std::string str) {
    std::unique_lock<std::mutex> lock(mutex);
    map.emplace(packet_no, str);
    map.erase(packet_no - buffer_size);
  }

  std::string at(uint64_t packet_no) {
    std::unique_lock<std::mutex> lock(mutex);
    auto it = map.find(packet_no);
    if (it == map.end()) {
      return std::string("");
    }
    return it->second;
  }
};
#endif
