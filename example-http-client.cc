/*
TODO: synchronous task - as a wrapper
TODO: server example
*/

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

synchronous_task http_client() {
  Socket s = Socket::ClientSocket(SockAddr(IPv4{}, "1.1.1.1", 80));
  if (auto status = co_await async_connect(s.without_timeout());
      status.err != 0) {
    throw std::system_error(status.err, std::system_category(), status.message);
  }

  std::string req =
      "GET / HTTP/1.1\r\nHost: 1.1.1.1\r\nConnection: close\r\n\r\n";
  if (auto status = co_await async_write(s.without_timeout(), req);
      status.err != 0) {
    throw std::system_error(status.err, std::system_category(), status.message);
  }

  co_return;

  /*

    std::cerr << "Wrote " << cnt << " bytes.\n";
    std::cerr << "Done writing.\n";

    char buff[512];
    while ((cnt = read(s.socket_, buff, 512)) != 0) {
      if (cnt == -1) {
        if (errno == EAGAIN) {
          std::cerr << "Waiting for read.\n";
          co_await socket_ready(s.socket_);
        } else {
          throw std::system_error(errno, std::system_category(),
                                  "Failed to read socket.");
        }
        continue;
      }
      for (ssize_t i = 0; i < cnt; i++) {
        std::cout << buff[i];
      }
    }*/
}

int main() { http_client(); }