#include "ces/coroutines/synchronous.h"

#include <iostream>

ces::main_task async_main(int argc, char *argv[]) {
  std::cout << "Arguments: \n";
  for (int i = 0; i < argc; i++) {
    std::cout << "\t" << argv[i] << "\n";
  }
  co_return 0;
}

int main(int argc, char *argv[]) { return async_main(argc, argv); }