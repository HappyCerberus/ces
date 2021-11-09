#include "utils/handle-error.h"

#include <system_error>

namespace ces::utils {

bool handle_error(int result, const char *errtxt) {
  if (result != -1)
    return true;
  if (errno == EAGAIN || errno == EINPROGRESS)
    return false;
  throw std::system_error(errno, std::system_category(), errtxt);
}

} // namespace ces::utils