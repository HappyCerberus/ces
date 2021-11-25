#include "ces/async/async-server.h"

#include "ces/async/async-accept.h"

namespace ces {

chainable_task
async_server(SocketView s,
             std::function<ces::terminating_task(Socket)> connection_handler) {
  while (true) {
    if (auto result = co_await async_accept(s, connection_handler); !result)
      co_return result;
  }
}

} // namespace ces