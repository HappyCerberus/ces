#ifndef CES_UTILS_TIME_FWD_H_
#define CES_UTILS_TIME_FWD_H_

#include <chrono>

namespace ces {
using clock = std::chrono::steady_clock;
using time_point = std::chrono::time_point<clock>;
using duration = std::chrono::duration<int64_t, std::micro>;
inline constexpr const auto NO_DEADLINE = time_point::max();
} // namespace ces

#endif