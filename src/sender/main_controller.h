#include <thread_safe_structures.h>
#include "sender_parameters_parser.h"

class MainController {
private:
  SenderParameters params;
  SafeFIFO *safe_fifo;
  SafeSet *safe_set;

public:
  MainController(SenderParameters params, SafeSet safe_set);

  void setup();

  void run();
}
