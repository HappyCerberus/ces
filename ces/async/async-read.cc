#include "ces/async/async-read.h"

#include "ces/coroutines/scheduler.h"

#include <unistd.h>

namespace ces {

chainable_task async_read(SocketView s, std::span<std::byte> &data) {
  auto shifting_data = data;
  while (shifting_data.size() > 0) {
    ssize_t cnt = read(s->fd_, shifting_data.data(), shifting_data.size());
    if (cnt == -1 && errno != EAGAIN)
      co_return utils::AwaitResult{utils::EventType::Error, errno,
                                   "Failed to read data from socket."};
    if (cnt > 0) {
      shifting_data = shifting_data.last(shifting_data.size() - cnt);
      continue;
    }
    if (cnt == 0) // EOF
      break;
    // cnt == -1 && errno == EAGAIN
    if (auto status = co_await main_scheduler().event(
            utils::EventCategory::EPOLL, s->fd_, s.deadline());
        !status) {
      co_return status;
    }
  }

  data = data.first(data.size() - shifting_data.size());
  co_return utils::AwaitResult{utils::EventType::WakeUp, 0, ""};
}
} // namespace ces