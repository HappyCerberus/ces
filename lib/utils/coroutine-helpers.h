#ifndef COROUTINE_HELPERS_H_
#define COROUTINE_HELPERS_H_

#include "lib/epoll-fwd.h"
#include <concepts>
#include <coroutine>
#include <functional>

namespace ces::utils {

// Concept for coroutine handles whose promise types have continuations.
template <typename T>
concept HasContinuation = requires(T x) {
  { x.promise().continuation } -> std::convertible_to<std::coroutine_handle<>>;
};

// Tail resume awaitable, designed to be returned from final_suspend().
struct awaitable_tail_resume {
  constexpr bool await_ready() const noexcept { return false; }
  std::coroutine_handle<>
  await_suspend(HasContinuation auto caller) const noexcept {
    return caller.promise().continuation;
  }
  constexpr void await_resume() const noexcept {}
};

// Trivial awaitable that hands off control to another coroutine.
struct awaitable_handoff {
  constexpr bool await_ready() const noexcept { return false; }
  std::coroutine_handle<> await_suspend(std::coroutine_handle<>) noexcept {
    return handle_;
  }
  constexpr void await_resume() const noexcept {}

  std::coroutine_handle<> handle_;
};

struct awaitable_epoll_handoff {
  constexpr bool await_ready() const noexcept { return false; }
  std::coroutine_handle<>
  await_suspend(std::coroutine_handle<> caller) noexcept {
    return epoll_handoff_coroutine(sock_, caller);
  }
  constexpr void await_resume() const noexcept {}
  SocketView sock_;
};

} // namespace ces::utils

#endif