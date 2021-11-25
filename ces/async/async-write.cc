#include "ces/async/async-write.h"

#include "ces/coroutines/scheduler.h"

#include <unistd.h>

namespace ces {

chainable_task async_write(SocketView s, std::span<const std::byte> &data) {
  while (data.size() > 0) {
    ssize_t cnt = write(s->fd_, data.data(), data.size());
    if (cnt == -1 && errno != EAGAIN)
      co_return utils::AwaitResult{utils::EventType::Error, errno,
                                   "Failed to write data to socket."};
    if (cnt >= 0) {
      data = data.last(data.size() - cnt);
      continue;
    }
    // cnt == -1 && errno == EAGAIN
    if (auto status = co_await main_scheduler().event(
            utils::EventCategory::EPOLL, s->fd_, s.deadline());
        !status) {
      co_return status;
    }
  }
  co_return utils::AwaitResult{utils::EventType::WakeUp, 0, ""};
}

} // namespace ces