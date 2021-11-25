#ifndef CES_ASYNC_ACCEPT_H_
#define CES_ASYNC_ACCEPT_H_

#include "ces/coroutines/chainable.h"
#include "ces/coroutines/terminating.h"
#include "ces/epoll/socket.h"

#include <functional>

namespace ces {

chainable_task
async_accept(SocketView s,
             std::function<ces::terminating_task(Socket)> connection_handler);

} // namespace ces

#endif