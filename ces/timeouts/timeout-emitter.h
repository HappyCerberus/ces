#ifndef CES_TIMEOUT_EMITTER_H_
#define CES_TIMEOUT_EMITTER_H_

#include "ces/event-emitter.h"
#include <set>

namespace ces {

struct TimeoutEmitter : public EventEmitter {
  TimeoutEmitter() : queue_(cmp_deadline) {}
  utils::AwaitableData *emit() override;
  void notify_arrival(utils::AwaitableData *data) override;
  void notify_departure(utils::AwaitableData *data) override;
  bool is_empty() override;

private:
  static bool cmp_deadline(utils::AwaitableData *l, utils::AwaitableData *r);
  std::multiset<utils::AwaitableData *, decltype(&cmp_deadline)> queue_;
};

} // namespace ces

#endif
