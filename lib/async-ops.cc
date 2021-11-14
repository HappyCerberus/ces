#include "lib/async-ops.h"

#include <unistd.h>

namespace ces {

chainable_task async_connect(SocketView s) {
  int ret = connect(s->fd_, s->addr_.get_addr(), s->addr_.get_len());
  if (ret == -1 && errno != EINPROGRESS) {
    co_return utils::Status{errno, 0, "Failed to connect to server."};
  }
  co_await s;
  co_return utils::Status{0, 0, ""};
}

chainable_task async_write(SocketView s, const std::string &rng) {
  ssize_t cnt =
      ::write(s->fd_, reinterpret_cast<const void *>(rng.data()), rng.length());
  if (cnt == -1 && errno != EAGAIN) {
    co_return utils::Status{errno, 0, "Failed to write data to socket."};
  }
  co_await s;
  co_return utils::Status{0, cnt, ""};
}

} // namespace ces