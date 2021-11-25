#ifndef CES_COROUTINES_AWAITABLES_H_
#define CES_COROUTINES_AWAITABLES_H_

#include <coroutine>

namespace ces {

struct awaitable_handoff {
  awaitable_handoff(std::coroutine_handle<> handle) : handle_(handle) {}
  constexpr bool await_ready() const noexcept { return false; }
  std::coroutine_handle<> await_suspend(std::coroutine_handle<>) noexcept {
    return handle_;
  }
  constexpr void await_resume() const noexcept {}

private:
  std::coroutine_handle<> handle_;
};

} // namespace ces

#endif