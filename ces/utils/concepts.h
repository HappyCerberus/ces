#ifndef CES_UTILS_CONCEPTS_H_
#define CES_UTILS_CONCEPTS_H_

#include <concepts>
#include <coroutine>

namespace ces {

template <typename T>
concept CoroutineHandle = std::convertible_to<T, std::coroutine_handle<>>;

template <typename T>
concept HandleWithContinuation = CoroutineHandle<T> && requires(T x) {
  { x.promise().continuation } -> std::convertible_to<std::coroutine_handle<>>;
};

template <typename T>
concept CoroutineWithContinuation = requires(T x) {
  { x.handle_ } -> HandleWithContinuation<>;
};

} // namespace ces

#endif