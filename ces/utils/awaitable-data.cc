#include "ces/utils/awaitable-data.h"

#include <ostream>

namespace ces::utils {

std::ostream &operator<<(std::ostream &s, const AwaitResult &res) {
  const char *types[] = {"None", "WakeUp", "Timeout", "Error"};
  s << "EventType: " << types[(int)res.result_type] << " Error: " << res.err
    << " Error Message: " << res.err_message;
  return s;
}

}