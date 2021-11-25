#ifndef CES_COROUTINES_CHAINABLE_H_
#define CES_COROUTINES_CHAINABLE_H_

#include "ces/utils/awaitable-data.h"
#include "ces/utils/concepts.h"

#include <coroutine>
#include <exception>
#include <utility>

namespace ces {

struct awaitable_chainable_continuation {
  constexpr bool await_ready() const noexcept { return false; }
  std::coroutine_handle<>
  await_suspend(HandleWithContinuation auto caller) const noexcept {
    return caller.promise().continuation;
  }
  constexpr void await_resume() const noexcept {}
};

// Chainable task for awaitable coroutines.
struct chainable_task {
  struct chainable_promise {
    using handle_t = std::coroutine_handle<chainable_promise>;
    chainable_task get_return_object() {
      return chainable_task{handle_t::from_promise(*this)};
    }
    std::suspend_always initial_suspend() noexcept { return {}; }
    awaitable_chainable_continuation final_suspend() noexcept { return {}; }
    void unhandled_exception() { std::terminate(); }
    void return_value(utils::AwaitResult result) { result_ = result; }

  private:
    std::coroutine_handle<> continuation = std::noop_coroutine();
    utils::AwaitResult result_;
    friend struct chainable_task;
    friend struct awaitable_chainable_continuation;
    friend struct scheduler_task;
  };

  using promise_type = chainable_promise;
  explicit chainable_task(promise_type::handle_t handle) : handle_(handle) {}
  chainable_task(const chainable_task &) = delete;
  chainable_task(chainable_task &&task)
      : handle_(std::exchange(task.handle_, nullptr)) {}
  chainable_task &operator=(const chainable_task &) = delete;
  chainable_task &operator=(chainable_task &&task) {
    handle_ = std::exchange(task.handle_, nullptr);
    return *this;
  }
  ~chainable_task() {
    if (handle_)
      handle_.destroy();
  }

  bool await_ready() noexcept { return false; }
  std::coroutine_handle<> await_suspend(std::coroutine_handle<> caller) {
    handle_.promise().continuation = caller;
    return handle_;
  }
  utils::AwaitResult await_resume() { return handle_.promise().result_; }

private:
  promise_type::handle_t handle_;
  friend struct scheduler_task;
};

} // namespace ces

#endif