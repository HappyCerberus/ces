#include "ces/async/async-connect.h"
#include "ces/coroutines/scheduler.h"

namespace ces {
chainable_task async_connect(SocketView s) {
  int ret = connect(s->fd_, s->addr_.get_addr(), s->addr_.get_len());
  if (ret == -1 && errno != EINPROGRESS)
    co_return utils::AwaitResult{utils::EventType::Error, errno,
                                 "Failed to connect to server."};
  if (ret == 0)
    co_return utils::AwaitResult{utils::EventType::WakeUp, 0, ""};
  auto epoll = co_await main_scheduler().event(utils::EventCategory::EPOLL,
                                               s->fd_, s.deadline());
  co_return epoll;
}
} // namespace ces