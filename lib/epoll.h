#ifndef EPOLL_H_
#define EPOLL_H_

#include "lib/socket.h"
#include "lib/utils/coroutine-helpers.h"
#include <coroutine>
#include <exception>
#include <unordered_map>

namespace ces {

struct SuspendedIO {
  SocketView socket;
  std::coroutine_handle<> handle;
};

struct WakeUpStatus {
  int fd;
  int err;
};

struct epoll_task {
  static constexpr struct get_promise_tag {
  } get_promise{};
  struct epoll_promise_type {
    using handle_type = std::coroutine_handle<epoll_promise_type>;
    epoll_task get_return_object() {
      return epoll_task{handle_type::from_promise(*this)};
    }
    std::suspend_always initial_suspend();
    std::suspend_never final_suspend() noexcept { return {}; }
    void unhandled_exception() { std::terminate(); }
    bool is_suspended(int fd);
    utils::awaitable_handoff yield_value(WakeUpStatus status);
    auto await_transform(get_promise_tag) {
      struct awaiter : std::suspend_never {
        promise_type &promise;

        promise_type &await_resume() { return promise; }
      };
      return awaiter{{}, *this};
    }

    int epoll_fd_;
    std::unordered_map<int, SuspendedIO> blocked_sockets_;
  };

  using promise_type = epoll_promise_type;
  using handle_type = promise_type::handle_type;
  explicit epoll_task(handle_type coro) : coro_(coro) {}
  ~epoll_task() {
    if (coro_) {
      coro_.destroy();
    }
  }

  handle_type coro_;
};

} // namespace ces

#endif