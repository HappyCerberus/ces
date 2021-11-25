#ifndef CES_ASYNC_CONNECT_H_
#define CES_ASYNC_CONNECT_H_

#include "ces/coroutines/chainable.h"
#include "ces/epoll/socket.h"

namespace ces {
chainable_task async_connect(SocketView s);
}

#endif