#ifndef CES_ASYNC_READ_H_
#define CES_ASYNC_READ_H_

#include "ces/coroutines/chainable.h"
#include "ces/epoll/socket.h"

#include <span>

namespace ces {
chainable_task async_read(SocketView s, std::span<std::byte> &data);
}

#endif