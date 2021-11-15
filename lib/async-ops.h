#ifndef ASYNC_OPS_H_
#define ASYNC_OPS_H_

#include "lib/coroutines.h"
#include "lib/socket.h"
#include <string>
#include <vector>

namespace ces {
chainable_task async_connect(SocketView s);
chainable_task async_write(SocketView s, const std::string &rng);
chainable_task async_read(SocketView s, std::vector<uint8_t> &rng);
} // namespace ces

#endif