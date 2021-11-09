#ifndef UTILS_PROMISE_ACCESSOR_H_
#define UTILS_PROMISE_ACCESSOR_H_

#include <coroutine>
#include <stdexcept>

namespace ces::utils {

/** Awaitable, that immediately resumes the awaiter,
    but remembers its handle and provides access
    to the promise instance.
**/
template <typename T> struct promise_accessor {
  bool await_ready() const { return false; }
  std::coroutine_handle<> await_suspend(std::coroutine_handle<T> handle) {
    handle_copy_ = handle;
    return handle;
  }
  void await_resume() const {}

  T &get_promise() {
    if (!handle_copy_ || handle_copy_.done())
      throw std::runtime_error(
          "Attempted to access promise on an expired coroutine handle.");
    return handle_copy_.promise();
  }

private:
  std::coroutine_handle<T> handle_copy_;
};

} // namespace ces::utils

#endif