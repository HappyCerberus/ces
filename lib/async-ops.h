#ifndef ASYNC_OPS_H_
#define ASYNC_OPS_H_

#include "lib/coroutines.h"
#include "lib/socket.h"
#include <string>

namespace ces {
chainable_task async_connect(SocketView s);
chainable_task async_write(SocketView s, const std::string &rng);
} // namespace ces

#endif