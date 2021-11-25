#include "ces/epoll/epoll-emitter.h"

#include <stdexcept>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <system_error>

namespace ces {
namespace {

int get_errno(int fd) {
  int error = 0;
  socklen_t errlen = sizeof(error);
  if (getsockopt(fd, SOL_SOCKET, SO_ERROR, (void *)&error, &errlen) == 0) {
    return error;
  } else {
    return errno;
  }
}

} // namespace

// When calling epoll_create, the paramter is ignored, but has to be >0
constexpr int IGNORED_EPOLL_PARAM = 1;

EpollEmitter::EpollEmitter() {
  if (epoll_fd_ != -1)
    return;
  epoll_fd_ = epoll_create(IGNORED_EPOLL_PARAM);
  if (epoll_fd_ == -1)
    throw std::system_error(errno, std::system_category(),
                            "Failed to create epoll socket.");
}

utils::AwaitableData *EpollEmitter::emit() {
  epoll_event ev;
  while (true) {
    int ret = epoll_wait(epoll_fd_, &ev, 1, 0);
    if (ret == -1)
      throw std::system_error(errno, std::system_category(),
                              "Failed to fetch epoll event.");

    if (ret == 0)
      return nullptr;

    auto it = awaiting_.find(ev.data.fd);
    if (it == awaiting_.end())
      continue;

    // epoll_event_dump(ev);
    if (ev.events & EPOLLERR) {
      it->second->result.err = get_errno(ev.data.fd);
      it->second->result.result_type = utils::EventType::Error;
      it->second->result.err_message = "Asynchronous socket error.";
    } else if (ev.events & (EPOLLHUP | EPOLLRDHUP)) {
      it->second->result.err = 0;
      it->second->result.err_message = "";
      it->second->result.result_type = utils::EventType::Hangup;
    } else if (ev.events & (EPOLLIN | EPOLLOUT)) {
      it->second->result.err = 0;
      it->second->result.err_message = "";
      it->second->result.result_type = utils::EventType::WakeUp;
    }
    return it->second;
  }
}

void EpollEmitter::notify_arrival(utils::AwaitableData *data) {
  if (data->event_category != utils::EventCategory::EPOLL)
    return;
  awaiting_.emplace(data->event_id, data);
}

void EpollEmitter::notify_departure(utils::AwaitableData *data) {
  if (data->event_category != utils::EventCategory::EPOLL)
    return;
  awaiting_.erase(data->event_id);
}

bool EpollEmitter::is_empty() { return awaiting_.empty(); }

int EpollEmitter::epoll_fd_ = -1;

void epoll_register_socket(int fd) {
  epoll_event ev = {};
  ev.events = EPOLLIN | EPOLLOUT | EPOLLPRI | EPOLLERR | EPOLLHUP | EPOLLET;
  ev.data.fd = fd;
  int ret = epoll_ctl(EpollEmitter::epoll_fd_, EPOLL_CTL_ADD, fd, &ev);
  if (ret == -1)
    throw std::system_error(errno, std::system_category(),
                            "Failed to register socket for epoll.");
}

void epoll_deregister_socket(int fd) {
  int ret = epoll_ctl(EpollEmitter::epoll_fd_, EPOLL_CTL_DEL, fd, nullptr);
  if (ret == -1)
    throw std::system_error(errno, std::system_category(),
                            "Failed to de-register socket from epoll.");
}

} // namespace ces