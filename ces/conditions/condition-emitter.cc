#include "ces/conditions/condition-emitter.h"

#include <algorithm>

namespace ces {
utils::AwaitableData *ConditionEmitter::emit() {
  auto it =
      std::ranges::find_if(awaiting_, [](auto *d) { return d->condition(); });
  if (it == awaiting_.end())
    return nullptr;
  (*it)->result.err = 0;
  (*it)->result.err_message = "";
  (*it)->result.result_type = utils::EventType::WakeUp;
  return *it;
}

void ConditionEmitter::notify_arrival(utils::AwaitableData *data) {
  if (data->condition)
    awaiting_.insert(data);
}

void ConditionEmitter::notify_departure(utils::AwaitableData *data) {
  if (data->condition)
    awaiting_.erase(data);
}

bool ConditionEmitter::is_empty() { return awaiting_.empty(); }

} // namespace ces