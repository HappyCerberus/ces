#include "ces/async/async-accept.h"
#include "ces/coroutines/scheduler.h"

#include <sys/socket.h>

namespace ces {

chainable_task
async_accept(SocketView s,
             std::function<ces::terminating_task(Socket)> connection_handler) {
  int ret;
  while ((ret = accept4(s->fd_, nullptr, nullptr, SOCK_NONBLOCK)) == -1) {
    if (errno != EAGAIN && errno != EWOULDBLOCK)
      co_return utils::AwaitResult{utils::EventType::Error, errno,
                                   "Failed to accept connection to server."};

    auto status = co_await main_scheduler().event(utils::EventCategory::EPOLL,
                                                  s->fd_, s.deadline());
    if (status.result_type != utils::EventType::WakeUp)
      co_return status;
  }
  auto handle = connection_handler(Socket::AcceptedSocket(ret));
  main_scheduler().schedule(handle);
  co_return utils::AwaitResult{};
}

} // namespace ces