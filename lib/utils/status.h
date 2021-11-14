#ifndef STATUS_H_
#define STATUS_H_

#include <cstdint>
#include <string>

namespace ces::utils {

struct Status {
  int err;             // 0 - OK, ETIME on timeout or errno reported by system.
  int64_t cnt_bytes;   // number of bytes written or read
  std::string message; // text error message if errno != 0
};

} // namespace ces::utils

#endif