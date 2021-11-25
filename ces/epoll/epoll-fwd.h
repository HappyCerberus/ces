#ifndef EPOLL_FWD_H_
#define EPOLL_FWD_H_

namespace ces {
void epoll_register_socket(int fd);
void epoll_deregister_socket(int fd);
} // namespace ces

#endif