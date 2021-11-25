#ifndef CES_COROUTINES_TERMINATING_H_
#define CES_COROUTINES_TERMINATING_H_

#include "ces/coroutines/awaitables/final-continuation.h"
#include <coroutine>

namespace ces {

struct terminating_task {
  struct terminating_promise {
    using handle_t = std::coroutine_handle<terminating_promise>;
    terminating_task get_return_object() {
      return terminating_task{handle_t::from_promise(*this)};
    }
    std::suspend_always initial_suspend() noexcept { return {}; }
    awaitable_final_continuation final_suspend() noexcept { return {}; }
    void unhandled_exception() { std::terminate(); }
    void return_void() {}
    std::coroutine_handle<> continuation = std::noop_coroutine();
  };
  using promise_type = terminating_promise;
  terminating_task(promise_type::handle_t handle) : handle_(handle) {}

private:
  promise_type::handle_t handle_;
  friend struct scheduler_task;
};

} // namespace ces

#endif