#ifndef EPOLL_FWD_H_
#define EPOLL_FWD_H_

#include "lib/socket.h"
#include <coroutine>

namespace ces {
void epoll_register_socket(int fd);
void epoll_deregister_socket(int fd);
std::coroutine_handle<> epoll_handoff_coroutine(SocketView sock,
                                                std::coroutine_handle<> handle);
} // namespace ces

#endif