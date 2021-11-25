#include "ces/coroutines/chainable.h"
#include "ces/coroutines/synchronous.h"

#include <iostream>

ces::chainable_task async_op(bool flag) {
  using namespace ces::utils;
  if (flag) {
    co_return AwaitResult{
        EventType::Error, 1,
        "Couldn't find the truth in the eye of the beholder."};
  } else {
    co_return AwaitResult{EventType::WakeUp, 0, ""};
  }
}

ces::synchronous_task demo() {
  if (auto ret = co_await async_op(true); !ret) {
    std::cerr << "Asynchronous operation failed: " << ret.err_message << "\n";
  }
  if (auto ret = co_await async_op(false); !ret) {
    std::cerr << "Unexpected failure: " << ret.err_message << "\n";
  }
  co_return;
}

int main() {
  demo();
  return 0;
}
