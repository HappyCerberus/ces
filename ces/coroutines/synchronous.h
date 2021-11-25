#ifndef CES_COROUTINES_SYNCHRONOUS_H_
#define CES_COROUTINES_SYNCHRONOUS_H_

#include <coroutine>
#include <exception>

namespace ces {

struct synchronous_task {
  struct synchronous_promise {
    synchronous_task get_return_object() { return {}; }
    std::suspend_never initial_suspend() noexcept { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void unhandled_exception() { std::terminate(); }
    void return_void() {}
  };
  using promise_type = synchronous_promise;
};

/* Variant of synchronous task designed for coroutine main
main_task asyc_main(int argc, char* argv[]) {
  // code
  return OK;
}

int main(int argc, char* argv[]) {
  return async_main(argc, argv);
}

*/
struct main_task {
  struct main_promise {
    using handle_t = std::coroutine_handle<main_promise>;
    main_task get_return_object() {
      return main_task{handle_t::from_promise(*this)};
    }
    std::suspend_never initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void unhandled_exception() { std::terminate(); }
    void return_value(int ret) { ret_ = ret; }

  private:
    int ret_;
    friend main_task;
  };
  using promise_type = main_promise;
  main_task(promise_type::handle_t handle) : handle_(handle) {}
  ~main_task() {
    if (handle_)
      handle_.destroy();
  }

  operator int() { return handle_.promise().ret_; }

private:
  promise_type::handle_t handle_;
};

} // namespace ces

#endif