#include "lib/epoll.h"
#include "lib/socket.h"

#include <coroutine>
#include <stdexcept>
#include <sys/epoll.h>
#include <system_error>

//#include <iostream>

namespace ces {
bool epoll_task::epoll_promise_type::is_suspended(int fd) {
  return blocked_sockets_.find(fd) != blocked_sockets_.end();
}

utils::awaitable_handoff
epoll_task::epoll_promise_type::yield_value(WakeUpStatus status) {
  auto it = blocked_sockets_.find(status.fd);
  if (it == blocked_sockets_.end())
    throw std::runtime_error("Internal inconsistency, socket handling "
                             "coroutine is not suspended.");
  it->second.socket->errno_ = status.err;
  std::coroutine_handle<> handle = it->second.handle;
  blocked_sockets_.erase(it);
  return utils::awaitable_handoff(handle);
}

// When calling epoll_create, the paramter is ignored, but has to be >0
constexpr int IGNORED_EPOLL_PARAM = 1;

std::suspend_always epoll_task::promise_type::initial_suspend() {
  epoll_fd_ = epoll_create(IGNORED_EPOLL_PARAM);
  if (epoll_fd_ == -1)
    throw std::system_error(errno, std::system_category(),
                            "Failed to create epoll socket.");
  return {};
}
/*
void epoll_event_dump(epoll_event &ev) {
  std::cerr << "Event for socket " << ev.data.fd << " :";
  if (ev.events & EPOLLIN)
    std::cerr << " EPOLLIN ";
  if (ev.events & EPOLLPRI)
    std::cerr << " EPOLLPRI";
  if (ev.events & EPOLLOUT)
    std::cerr << " EPOLLOUT";
  if (ev.events & EPOLLRDNORM)
    std::cerr << " EPOLLRDNORM";
  if (ev.events & EPOLLRDBAND)
    std::cerr << " EPOLLRDBAND";
  if (ev.events & EPOLLWRNORM)
    std::cerr << " EPOLLWRNORM";
  if (ev.events & EPOLLWRBAND)
    std::cerr << " EPOLLWRBAND";
  if (ev.events & EPOLLMSG)
    std::cerr << " EPOLLMSG";
  if (ev.events & EPOLLERR)
    std::cerr << " EPOLLERR";
  if (ev.events & EPOLLHUP)
    std::cerr << " EPOLLHUP";
  if (ev.events & EPOLLRDHUP)
    std::cerr << " EPOLLRDHUP";
  if (ev.events & EPOLLWAKEUP)
    std::cerr << " EPOLLWAKEUP";
  if (ev.events & EPOLLONESHOT)
    std::cerr << " EPOLLONESHOT";
  if (ev.events & EPOLLET)
    std::cerr << " EPOLLET";
  std::cerr << "\n";
}
*/
epoll_task epoll_loop() {
  auto &promise = co_await epoll_task::get_promise;

  epoll_event ev;
  while (true) {
    // read one epoll event, don't wait for it
    int ret = epoll_wait(promise.epoll_fd_, &ev, 1, 0);
    if (ret == -1)
      throw std::system_error(errno, std::system_category(),
                              "Failed to fetch epoll event.");
    if (ret == 0) {
      continue;
    }
    int fd = ev.data.fd; // we can't take reference of ev.data.fd directly
    if (!promise.is_suspended(fd)) {
      continue; // no coroutine handle to resume
    }
    // epoll_event_dump(ev);
    if (ev.events & EPOLLERR) {
      int err = get_errno(fd);
      co_yield WakeUpStatus{fd, err};
    } else if (ev.events & (EPOLLHUP | EPOLLRDHUP)) {
      co_yield WakeUpStatus{fd, -1};
    } else if (ev.events & (EPOLLIN | EPOLLOUT)) {
      co_yield WakeUpStatus{fd, 0};
    }
  }
}

std::coroutine_handle<epoll_task::epoll_promise_type> epoll_instance() {
  static epoll_task epoll_ = epoll_loop();
  return epoll_.coro_;
}

void epoll_register_socket(int fd) {
  auto handle = epoll_instance();

  epoll_event ev = {};
  ev.events = EPOLLIN | EPOLLOUT | EPOLLPRI | EPOLLERR | EPOLLHUP | EPOLLET;
  ev.data.fd = fd;
  int ret = epoll_ctl(handle.promise().epoll_fd_, EPOLL_CTL_ADD, fd, &ev);
  if (ret == -1)
    throw std::system_error(errno, std::system_category(),
                            "Failed to register socket for epoll.");
}

void epoll_deregister_socket(int fd) {
  auto handle = epoll_instance();
  int ret = epoll_ctl(handle.promise().epoll_fd_, EPOLL_CTL_DEL, fd, nullptr);
  if (ret == -1)
    throw std::system_error(errno, std::system_category(),
                            "Failed to de-register socket from epoll.");
}

std::coroutine_handle<>
epoll_handoff_coroutine(SocketView socket, std::coroutine_handle<> handle) {
  std::coroutine_handle<epoll_task::epoll_promise_type> eh = epoll_instance();
  eh.promise().blocked_sockets_.emplace(socket->fd_,
                                        SuspendedIO{socket, handle});
  return eh;
}

} // namespace ces
