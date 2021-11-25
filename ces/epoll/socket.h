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

  const struct sockaddr *get_addr() const;
  socklen_t get_len() const;

  std::variant<sockaddr_in, sockaddr_in6> addr_;
};

struct Socket;

using SocketView = utils::ObserverWithDeadline<Socket>;

struct Socket {
  static Socket ServerSocket(SockAddr addr);
  static Socket ClientSocket(SockAddr addr);
  SocketView without_timeout() { return SocketView(*this); }
  SocketView with_timeout(duration timeout) {
    return SocketView(*this, clock::now() + timeout);
  }

  Socket(SockAddr addr);
  ~Socket();

  SockAddr addr_;
  int fd_;
};

} // namespace ces

#endif