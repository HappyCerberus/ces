#ifndef CES_CONDITION_EMITTER_H_
#define CES_CONDITION_EMITTER_H_

#include "ces/event-emitter.h"

#include <unordered_set>

namespace ces {

struct ConditionEmitter : public EventEmitter {
  utils::AwaitableData *emit() override;
  void notify_arrival(utils::AwaitableData *data) override;
  void notify_departure(utils::AwaitableData *data) override;
  bool is_empty() override;

private:
  std::unordered_set<utils::AwaitableData *> awaiting_;
};

} // namespace ces

#endif