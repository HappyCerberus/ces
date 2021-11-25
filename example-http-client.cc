#include <cerrno>
#include <iostream>
#include <ranges>
#include <span>
#include <string.h>
#include <system_error>

#include "ces/async/async-connect.h"
#include "ces/async/async-read.h"
#include "ces/async/async-write.h"
#include "ces/coroutines/scheduler.h"
#include "ces/epoll/epoll-emitter.h"
#include "ces/epoll/socket.h"
#include "ces/timeouts/timeout-emitter.h"

using namespace std::chrono_literals;

ces::chainable_task http_client(const char *ip, int port) {
  ces::Socket s =
      ces::Socket::ClientSocket(ces::SockAddr(ces::IPv4{}, ip, port));
  if (auto status = co_await ces::async_connect(s.with_timeout(200ms));
      status.err != 0) {
    throw std::system_error(status.err, std::system_category(),
                            status.err_message);
  }

  std::cerr << "Connected!\n";

  std::string req =
      "GET / HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n";
  auto wdata = std::as_bytes(std::span(req));
  if (auto status = co_await async_write(s.with_timeout(200ms), wdata);
      status.err != 0) {
    throw std::system_error(status.err, std::system_category(),
                            status.err_message);
  }
  if (wdata.size() > 0)
    throw std::runtime_error("Failed to write entire request.");

  std::cerr << "Wrote request!\n";

  std::vector<uint8_t> buff(4096, 0);
  auto rdata = std::as_writable_bytes(std::span(buff));
  if (auto status = co_await async_read(s.with_timeout(200ms), rdata);
      status.err != 0) {
    throw std::system_error(status.err, std::system_category(),
                            status.err_message);
  } else {
    for (auto b : rdata) {
      std::cout << (char)b;
    }
    std::cout << "\n";
  }

  co_return ces::utils::AwaitResult{};
}

int main() {
  auto &sched = ces::main_scheduler();
  sched.register_emitter(std::make_unique<ces::TimeoutEmitter>());
  sched.register_emitter(std::make_unique<ces::EpollEmitter>());
  auto client = http_client("127.0.0.1", 8000);
  sched.schedule(client);
  sched.run();
}