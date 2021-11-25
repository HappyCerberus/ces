#ifndef CES_ASYNC_WRITE_H_
#define CES_ASYNC_WRITE_H_

#include "ces/coroutines/chainable.h"
#include "ces/epoll/socket.h"

#include <span>

namespace ces {
chainable_task async_write(SocketView s, std::span<const std::byte> &data);
}

#endif