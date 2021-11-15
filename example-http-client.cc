#include "lib/async-ops.h"
#include "lib/coroutines.h"
#include "lib/socket.h"
#include "lib/utils/coroutine-helpers.h"
#include "lib/utils/status.h"
#include <cerrno>
#include <iostream>
#include <ranges>
#include <string.h>
#include <system_error>

using namespace ces;

synchronous_task http_client(const char *ip, int port) {
  Socket s = Socket::ClientSocket(SockAddr(IPv4{}, ip, port));
  if (auto status = co_await async_connect(s.without_timeout());
      status.err != 0) {
    throw std::system_error(status.err, std::system_category(), status.message);
  }

  std::string req =
      "GET / HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n";
  if (auto status = co_await async_write(s.without_timeout(), req);
      status.err != 0) {
    throw std::system_error(status.err, std::system_category(), status.message);
  }

  std::vector<uint8_t> buff(512, 0);
  if (auto status = co_await async_read(s.without_timeout(), buff);
      status.err != 0) {
    throw std::system_error(status.err, std::system_category(), status.message);
  } else {
    for (size_t i = 0; i < status.cnt_bytes; i++) {
      std::cout << buff[i];
    }
    std::cout << "\n";
  }

  co_return;
}

int main() { http_client("127.0.0.1", 8000); }