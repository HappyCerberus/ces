#include "ces/coroutines/scheduler.h"

namespace ces {

void scheduler_task::register_emitter(std::unique_ptr<EventEmitter> emitter) {
  handle_.promise().emitters_.push_back(move(emitter));
}

namespace {
scheduler_task main_scheduler_impl() {
  auto &promise = co_await scheduler_task::get_promise;
  while (true) {
    while (promise.scheduled_.size() > 0) {
      co_await awaitable_handoff(promise.scheduled_.front());
      if (promise.scheduled_.front().done()) {
        promise.scheduled_.front().destroy();
      }
      promise.scheduled_.pop_front();
    }

    bool all_done = true;
    for (auto &em : promise.emitters_) {
      if (!em->is_empty()) {
        all_done = false;
      }
    }
    if (all_done) {
      co_yield nullptr;
    }

    for (auto &em : promise.emitters_) {
      for (auto *ev = em->emit(); ev != nullptr; ev = em->emit()) {
        co_yield ev;
      }
    }
  }
}

} // namespace

scheduler_task &main_scheduler() {
  static scheduler_task task = main_scheduler_impl();
  return task;
}

void notify_emitters(utils::AwaitableData *data) {
  main_scheduler().notify_emitters(data);
}

void scheduler_task::notify_emitters(utils::AwaitableData *data) {
  for (auto &em : handle_.promise().emitters_) {
    em->notify_arrival(data);
  }
}

universal_awaiter scheduler_task::event(utils::EventCategory category,
                                        utils::EventID id, duration timeout) {
  return event(category, id, clock::now() + timeout);
}

universal_awaiter scheduler_task::event(utils::EventCategory category,
                                        utils::EventID id,
                                        time_point deadline) {
  return universal_awaiter{utils::AwaitableData(category, id, deadline),
                           handle_};
}

universal_awaiter scheduler_task::condition(std::function<bool()> cond,
                                            duration timeout) {
  return condition(std::move(cond), clock::now() + timeout);
}

universal_awaiter scheduler_task::condition(std::function<bool()> cond,
                                            time_point deadline) {
  return universal_awaiter{utils::AwaitableData(std::move(cond), deadline),
                           handle_};
}

} // namespace ces