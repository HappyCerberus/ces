#ifndef CES_COROUTINES_SCHEDULER_H_
#define CES_COROUTINES_SCHEDULER_H_

#include "ces/coroutines/awaitables/handoff.h"
#include "ces/coroutines/awaitables/universal.h"
#include "ces/event-emitter.h"
#include "ces/utils/awaitable-data.h"
#include "ces/utils/concepts.h"

#include <chrono>
#include <coroutine>
#include <deque>
#include <memory>
#include <vector>

namespace ces {

struct scheduler_task {
  static constexpr struct get_promise_tag {
  } get_promise{};
  struct scheduler_promise {
    using handle_t = std::coroutine_handle<scheduler_promise>;
    scheduler_task get_return_object() {
      return scheduler_task{handle_t::from_promise(*this)};
    }
    std::suspend_always initial_suspend() noexcept { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void unhandled_exception() { std::terminate(); }
    awaitable_handoff yield_value(utils::AwaitableData *data) {
      if (data == nullptr)
        return awaitable_handoff(std::noop_coroutine());

      for (auto &em : emitters_) {
        em->notify_departure(data);
      }
      return awaitable_handoff(data->continuation);
    }

    auto await_transform(get_promise_tag) {
      struct awaiter : std::suspend_never {
        promise_type &promise;

        promise_type &await_resume() { return promise; }
      };
      return awaiter{{}, *this};
    }
    template <typename U> U &&await_transform(U &&awaitable) noexcept {
      return std::forward<U &&>(awaitable);
    }

    std::deque<std::coroutine_handle<>> scheduled_;
    std::vector<std::unique_ptr<EventEmitter>> emitters_;
  };
  using promise_type = scheduler_promise;
  explicit scheduler_task(promise_type::handle_t handle) : handle_(handle) {}

  scheduler_task(const scheduler_task &) = delete;
  scheduler_task(scheduler_task &&) = delete;
  scheduler_task &operator=(const scheduler_task &) = delete;
  scheduler_task &operator=(scheduler_task &&) = delete;

  void schedule(CoroutineWithContinuation auto &coro) {
    handle_.promise().scheduled_.push_back(coro.handle_);
    coro.handle_.promise().continuation = handle_;
  }
  void register_emitter(std::unique_ptr<EventEmitter> emitter);
  void run() { handle_.resume(); }

  universal_awaiter event(utils::EventCategory category, utils::EventID id,
                          duration timeout);
  universal_awaiter event(utils::EventCategory category, utils::EventID id,
                          time_point deadline = NO_DEADLINE);
  universal_awaiter condition(std::function<bool()> condition,
                              duration timeout);
  universal_awaiter condition(std::function<bool()> condition,
                              time_point deadline = NO_DEADLINE);

private:
  promise_type::handle_t handle_;
  void notify_emitters(utils::AwaitableData *);
  friend void notify_emitters(utils::AwaitableData *);
};

scheduler_task &main_scheduler();

} // namespace ces

#endif