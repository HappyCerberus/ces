#include <cerrno>
#include <coroutine>
#include <cstdint>
#include <deque>
#include <system_error>
#include <unordered_map>
#include <utility>
#include <vector>

// POSIX / LINUX
#include <unistd.h>

struct task {
  struct promise_type {
    task get_return_object() {
      return task{std::coroutine_handle<promise_type>::from_promise(*this)};
    }
    std::suspend_never initial_suspend() { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() {}
  };

  explicit task(std::coroutine_handle<promise_type> coro)
      : coro_(std::move(coro)) {}

private:
  std::coroutine_handle<promise_type> coro_;
};

struct async_io {
  struct promise_type {
    async_io get_return_object() {
      return async_io{std::coroutine_handle<promise_type>::from_promise(*this)};
    }
    std::suspend_never initial_suspend() { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() {}
  };

  bool await_ready() {}
  std::coroutine_handle<> await_suspend(std::coroutine_handle<>) {}
  void await_resume() {}

  explicit async_io(std::coroutine_handle<promise_type> coro)
      : coro_(std::move(coro)) {}

private:
  std::coroutine_handle<promise_type> coro_;
};

std::unordered_map<int, std::coroutine_handle<async_io::promise_type>>
    blocked_ops;

// epoll await, switches control to epoll coroutine
// epol coroutine - global handle

struct epoll_handle {
  struct promise_type {
    epoll_handle get_return_object() {
      return epoll_handle{
          std::coroutine_handle<promise_type>::from_promise(*this)};
    }
    std::suspend_always initial_suspend() { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() {}
  };

  explicit epoll_handle(std::coroutine_handle<promise_type> coro)
      : coro_(std::move(coro)) {}

private:
  std::coroutine_handle<promise_type> coro_;
};

#include <sys/epoll.h>

#include "utils/handle-error.h"

#include <cassert>

async_epoll_handle epoll_loop() {
  int epoll_ = epoll_create(1 /*ignored*/);
  // epoll_create can't return EAGAIN or EINPROGRESS
  assert(ces::utils::handle_error(epoll_, "Failed to create epoll instance."));

  epoll_event ev;
  while (true) {
    // read one epoll event, don't wait for it
    int ret = epoll_wait(epoll_, &ev, 1, 0);
    // epoll_create can't return EAGAIN or EINPROGRESS
    assert(ces::utils::handle_error(ret, "Failed to fetch epoll event."));
    // if error event - then we need special handling
    // if IN/OUT event - then remove the handle for that coroutine from the map
    //                 - yield that handle
    //                 - in the yield method, store this so the co_await can
    //                 resume it
  }
}

auto async_epoll_handle = epoll_loop(async_epoll_handle::init());

struct async_epoll_t {
  async_epoll_t(int fd) : fd_(fd) {}
  bool await_ready() { return false; }
  std::coroutine_handle<>
  await_suspend(std::coroutine_handle<async_io::promise_type> handle) {
    blocked_ops.emplace(fd_, std::move(handle));
    return async_epoll_handle;
  }
  void await_resume() {}

private:
  int fd_;
};

async_io async_read(int fd, std::vector<uint8_t> data) {
  int ret;
  uint8_t *head = data.data();
  size_t cnt_rem = data.size();
  while (cnt_rem > 0 && (ret = read(fd, head, cnt_rem)) != 0) {
    if (ret == -1 && errno == EAGAIN) {
      co_await async_epoll_t(fd);
    }
  }
}

task wrap() {}

int main() {}