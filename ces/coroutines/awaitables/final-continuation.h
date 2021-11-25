#ifndef CES_COROUTINES_AWAITABLE_FINAL_CONTINUATION_H_
#define CES_COROUTINES_AWAITABLE_FINAL_CONTINUATION_H_

#include "ces/utils/concepts.h"

#include <coroutine>

namespace ces {

struct awaitable_final_continuation {
  constexpr bool await_ready() const noexcept { return false; }
  std::coroutine_handle<>
  await_suspend(HandleWithContinuation auto caller) const noexcept {
    return caller.promise().continuation;
  }
  constexpr void await_resume() const noexcept {}
};

} // namespace ces

#endif