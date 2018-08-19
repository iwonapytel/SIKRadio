void print_vec(std::vector<uint64_t> vec) {
  for (auto it = vec.begin(); it != vec.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;
}

void safe_set_test() {
  SafeSet safe_set;
  safe_set.add(1);
  safe_set.add(2);
  safe_set.add(3);
  auto v = safe_set.get_all();
  std::cout << "v: 1 2 3" << std::endl;
  print_vec(v);
  auto v2 = safe_set.get_all();
  std::cout << "v2: empty" << std::endl;
  print_vec(v2);
}

void safe_fifo_test() {
  SafeFIFO fifo(2, 2);
  fifo.push(0, "a");
  fifo.push(1, "b");
  fifo.push(2, "c");
  fifo.push(3, "d");
  fifo.push(4, "e");
  assert(fifo.at(2) == "c");
  assert(fifo.at(0) == "");
  std::cout << "safefifo at 3: " << fifo.at(3) << std::endl;
 }
