#ifndef COROUTINES_H_
#define COROUTINES_H_

#include "lib/utils/coroutine-helpers.h"
#include "lib/utils/status.h"
#include <coroutine>
#include <iostream>

namespace ces {

// Chainable task for awaitable coroutines.
struct chainable_task {
  struct chainable_promise_type {
    using handle_type = std::coroutine_handle<chainable_promise_type>;
    chainable_task get_return_object() {
      return chainable_task{handle_type::from_promise(*this)};
    }
    std::suspend_always initial_suspend() noexcept { return {}; }
    ces::utils::awaitable_tail_resume final_suspend() noexcept { return {}; }
    void unhandled_exception() { std::terminate(); }
    void return_value(ces::utils::Status s) { status = s; }
    ces::utils::awaitable_epoll_handoff
    await_transform(ces::SocketView socket) {
      return ces::utils::awaitable_epoll_handoff{socket};
    }
    template <typename U> U &&await_transform(U &&awaitable) noexcept {
      return std::forward<U &&>(awaitable);
    }
    std::coroutine_handle<> continuation = std::noop_coroutine();
    ces::utils::Status status;
  };

  using promise_type = chainable_promise_type;
  using handle_type = promise_type::handle_type;
  explicit chainable_task(handle_type coro) : coro_(coro) {}
  chainable_task(const chainable_task &) = delete;
  chainable_task(chainable_task &&task) : coro_(move(task.coro_)) {
    task.coro_ = nullptr;
  }
  chainable_task &operator=(const chainable_task &) = delete;
  chainable_task &operator=(chainable_task &&task) {
    coro_ = task.coro_;
    task.coro_ = nullptr;
    return *this;
  }
  ~chainable_task() {
    if (coro_) {
      coro_.destroy();
    }
  }

  bool await_ready() noexcept { return false; }
  std::coroutine_handle<> await_suspend(std::coroutine_handle<> caller) {
    coro_.promise().continuation = caller;
    return coro_;
  }
  ces::utils::Status await_resume() { return coro_.promise().status; }

  handle_type coro_;
};

struct synchronous_task {
  struct synchronous_promise_type {
    synchronous_task get_return_object() { return {}; }
    std::suspend_never initial_suspend() noexcept { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void unhandled_exception() { std::terminate(); }
    void return_void() {}
  };
  using promise_type = synchronous_promise_type;
};

struct scheduled_task {
  struct scheduled_promise_type {
    using handle_type = std::coroutine_handle<scheduled_promise_type>;
    scheduled_task get_return_object() {
      return scheduled_task{handle_type::from_promise(*this)};
    }
    std::suspend_always initial_suspend() noexcept {
      // schedule this coroutine on global queue
      return {};
    }
    std::suspend_never final_suspend() noexcept {
      // TODO: resume the epoll loop
      return {};
    }
    void unhandled_exception() { std::terminate(); }
    void return_void() {}
  };

  using promise_type = scheduled_promise_type;
  using handle_type = promise_type::handle_type;
  explicit scheduled_task(handle_type coro) : coro_(coro) {}
  scheduled_task(const scheduled_task &) = delete;
  scheduled_task(scheduled_task &&task) : coro_(move(task.coro_)) {
    task.coro_ = nullptr;
  }
  scheduled_task &operator=(const scheduled_task &) = delete;
  scheduled_task &operator=(scheduled_task &&task) {
    coro_ = task.coro_;
    task.coro_ = nullptr;
    return *this;
  }
  ~scheduled_task() {
    if (coro_) {
      coro_.destroy();
    }
  }

  handle_type coro_;
};

} // namespace ces

#endif