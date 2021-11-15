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
  size_t bytes = 0;
  const char *head = rng.data();
  ssize_t cnt;
  while (bytes < rng.length()) {
    ssize_t cnt = write(s->fd_, head, rng.length() - bytes);
    if (cnt == -1 && errno != EAGAIN) {
      co_return utils::Status{errno, 0, "Failed to write data to socket."};
    }
    if (cnt == -1) { // EAGAIN
      co_await s;
      continue;
    }
    bytes += cnt;
    head += cnt;
  }
  co_return utils::Status{0, static_cast<size_t>(cnt), ""};
}

chainable_task async_read(SocketView s, std::vector<uint8_t> &rng) {
  size_t bytes = 0;
  uint8_t *head = rng.data();
  ssize_t cnt;
  while (bytes < rng.size() &&
         (cnt = read(s->fd_, head, rng.size() - bytes)) != 0) {
    if (cnt == -1 && errno != EAGAIN) {
      co_return utils::Status{errno, 0, "Failed to read data from socket."};
    }
    if (cnt == -1) { // EAGAIN
      co_await s;
      continue;
    }
    head += cnt;
    bytes += cnt;
  }
  co_return utils::Status{0, bytes, ""};
}

} // namespace ces