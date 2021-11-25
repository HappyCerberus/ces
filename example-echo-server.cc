#include <cerrno>
#include <iostream>
#include <ranges>
#include <span>
#include <string.h>
#include <system_error>

#include "ces/async/async-connect.h"
#include "ces/async/async-read.h"
#include "ces/async/async-server.h"
#include "ces/async/async-write.h"
#include "ces/conditions/condition-emitter.h"
#include "ces/coroutines/scheduler.h"
#include "ces/coroutines/terminating.h"
#include "ces/epoll/epoll-emitter.h"
#include "ces/epoll/socket.h"
#include "ces/timeouts/timeout-emitter.h"

using namespace std::chrono_literals;

std::span<std::byte> to_read_buff(auto &data) {
  return std::as_writable_bytes(std::span(data));
}

std::span<const std::byte> to_write_buff(auto &data) {
  return std::as_bytes(std::span(data));
}

ces::terminating_task echo_server_handle(ces::Socket s) {
  uint32_t rd_size[1] = {0};
  auto rd_buff = to_read_buff(rd_size);
  if (auto status = co_await async_read(s.without_timeout(), rd_buff);
      status.err != 0)
    throw std::system_error(status.err, std::system_category(),
                            status.err_message);

  std::cout << "Server | read length of request " << rd_size[0] << "\n";

  std::string rd_text(rd_size[0], ' ');
  rd_buff = to_read_buff(rd_text);
  if (auto status = co_await async_read(s.without_timeout(), rd_buff);
      status.err != 0)
    throw std::system_error(status.err, std::system_category(),
                            status.err_message);

  std::cout << "Server | read content of request \"" << rd_text << "\"\n";

  auto wr_buff = to_write_buff(rd_size);
  if (auto status = co_await async_write(s.without_timeout(), wr_buff);
      status.err != 0)
    throw std::system_error(status.err, std::system_category(),
                            status.err_message);
  std::cout << "Server | wrote response length\n";

  wr_buff = to_write_buff(rd_text);
  if (auto status = co_await async_write(s.without_timeout(), wr_buff);
      status.err != 0)
    throw std::system_error(status.err, std::system_category(),
                            status.err_message);
  std::cout << "Server | wrote response content\n";

  co_return;
}

ces::terminating_task echo_client(ces::SocketView server_socket) {
  if (auto status = co_await ces::main_scheduler().condition(
          [server_socket]() { return server_socket->operational_; });
      !status)
    throw std::system_error(status.err, std::system_category(),
                            status.err_message);

  ces::Socket s = server_socket->make_client();
  if (auto status = co_await ces::async_connect(s.without_timeout());
      status.err != 0) {
    throw std::system_error(status.err, std::system_category(),
                            status.err_message);
  }

  std::string wr_text = "Hello World!";
  uint32_t wr_size[1] = {static_cast<uint32_t>(wr_text.length())};
  auto wr_buff = to_write_buff(wr_size);
  if (auto status = co_await async_write(s.without_timeout(), wr_buff);
      status.err != 0)
    throw std::system_error(status.err, std::system_category(),
                            status.err_message);
  std::cout << "Echo Client | wrote request length\n";

  wr_buff = to_write_buff(wr_text);
  if (auto status = co_await async_write(s.without_timeout(), wr_buff);
      status.err != 0)
    throw std::system_error(status.err, std::system_category(),
                            status.err_message);
  std::cout << "Echo Client | wrote request content\n";

  uint32_t rd_size[1] = {0};
  auto rd_buff = to_read_buff(rd_size);
  if (auto status = co_await async_read(s.without_timeout(), rd_buff);
      status.err != 0)
    throw std::system_error(status.err, std::system_category(),
                            status.err_message);
  std::cout << "Echo Client | got reponse length " << rd_size[0] << "\n";

  std::string rd_text(rd_size[0], ' ');
  rd_buff = to_read_buff(rd_text);
  if (auto status = co_await async_read(s.without_timeout(), rd_buff);
      status.err != 0)
    throw std::system_error(status.err, std::system_category(),
                            status.err_message);
  std::cout << "Echo Client | got data \"" << rd_text << "\"\n";

  co_return;
}

int main() {
  auto &sched = ces::main_scheduler();
  sched.register_emitter(std::make_unique<ces::TimeoutEmitter>());
  sched.register_emitter(std::make_unique<ces::ConditionEmitter>());
  sched.register_emitter(std::make_unique<ces::EpollEmitter>());

  ces::Socket s =
      ces::Socket::ServerSocket(ces::SockAddr(ces::IPv4{}, "127.0.0.1", 9090));
  auto server = async_server(s.without_timeout(), echo_server_handle);
  auto client = echo_client(s.without_timeout());
  sched.schedule(server);
  sched.schedule(client);
  sched.run();

  std::cout << "Main done.\n";
}