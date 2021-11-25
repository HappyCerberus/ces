#ifndef CES_EPOLL_EMITTER_H_
#define CES_EPOLL_EMITTER_H_

#include "ces/event-emitter.h"

#include <unordered_map>

namespace ces {

struct EpollEmitter : public EventEmitter {
  EpollEmitter();
  utils::AwaitableData *emit() override;
  void notify_arrival(utils::AwaitableData *data) override;
  void notify_departure(utils::AwaitableData *data) override;
  bool is_empty() override;

private:
  std::unordered_map<int, utils::AwaitableData *> awaiting_;
  static int epoll_fd_;
  friend void epoll_register_socket(int fd);
  friend void epoll_deregister_socket(int fd);
};

} // namespace ces

#endif