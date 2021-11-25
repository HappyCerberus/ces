#include "ces/timeouts/timeout-emitter.h"

namespace ces {

utils::AwaitableData *TimeoutEmitter::emit() {
  if (queue_.size() != 0) {
    auto v = *queue_.begin();
    if (v->deadline < clock::now()) {
      v->result.result_type = utils::EventType::Timeout;
      v->result.err = ETIMEDOUT;
      v->result.err_message = "Timed out by TimeoutEmitter.";
      return v;
    }
  }
  return nullptr;
}

void TimeoutEmitter::notify_arrival(utils::AwaitableData *data) {
  if (data->deadline != NO_DEADLINE)
    queue_.insert(data);
}

void TimeoutEmitter::notify_departure(utils::AwaitableData *data) {
  auto [b, e] = queue_.equal_range(data);
  for (auto i = b; i != e; i++) {
    if (*i == data) {
      queue_.erase(i);
      return;
    }
  }
}

bool TimeoutEmitter::cmp_deadline(utils::AwaitableData *l,
                                  utils::AwaitableData *r) {
  return l->deadline < r->deadline;
}

bool TimeoutEmitter::is_empty() { return queue_.empty(); }

} // namespace ces