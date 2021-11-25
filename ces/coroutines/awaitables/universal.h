#ifndef CES_COROUTINES_AWAITABLES_UNIVERSAL_H_
#define CES_COROUTINES_AWAITABLES_UNIVERSAL_H_

#include "ces/utils/awaitable-data.h"

#include <coroutine>

namespace ces {

void notify_emitters(utils::AwaitableData *);

struct universal_awaiter {
  utils::AwaitableData data_;
  std::coroutine_handle<> handle_;
  bool await_ready() {
    if (!data_.condition)
      return false;
    bool result = data_.condition();
    if (result)
      data_.result.result_type = utils::EventType::WakeUp;
    return result;
  }
  std::coroutine_handle<> await_suspend(std::coroutine_handle<> caller) {
    data_.continuation = caller;
    notify_emitters(&data_);
    return handle_;
  }
  utils::AwaitResult await_resume() { return data_.result; }
};

} // namespace ces

#endif