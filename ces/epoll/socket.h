#ifndef SOCKET_H_
#define SOCKET_H_

#include "ces/utils/observer.h"

#include <arpa/inet.h>
#include <variant>

namespace ces {
struct IPv4 {};
struct IPv6 {};

struct SockAddr {
  SockAddr(IPv4, const char *addr, int port);
  SockAddr(IPv6, const char *addr, int port);
  SockAddr() {}

  const struct sockaddr *get_addr() const;
  socklen_t get_len() const;

  std::variant<sockaddr_in, sockaddr_in6> addr_;
};

struct Socket;

using SocketView = utils::ObserverWithDeadline<Socket>;

struct Socket {
  static Socket ServerSocket(SockAddr addr);
  static Socket ClientSocket(SockAddr addr);
  static Socket AcceptedSocket(int fd);
  SocketView without_timeout() { return SocketView(*this); }
  SocketView with_timeout(duration timeout) {
    return SocketView(*this, clock::now() + timeout);
  }

  Socket(SockAddr addr);
  Socket(int fd);
  ~Socket();
  Socket(const Socket &) = delete;
  Socket(Socket &&s)
      : addr_(std::exchange(s.addr_, SockAddr{})),
        fd_(std::exchange(s.fd_, -1)) {}

  Socket &operator=(const Socket &s) = delete;
  Socket &operator=(Socket &&s) {
    addr_ = std::exchange(s.addr_, SockAddr{});
    fd_ = std::exchange(s.fd_, -1);
    return *this;
  }

  Socket make_client();

  SockAddr addr_;
  int fd_;
  bool operational_ = false;
};

} // namespace ces

#endif