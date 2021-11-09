#ifndef UTILS_HANDLE_ERROR_H_
#define UTILS_HANDLE_ERROR_H_

#include <string_view>

namespace ces::utils {

/* Translates result & errno

    Returns true if result >= 0
    false if errno is EAGAIN or EINPROGRESS
    throws otherwise
*/
[[nodiscard]] bool handle_error(int result, const char *errtxt);

} // namespace ces::utils

#endif