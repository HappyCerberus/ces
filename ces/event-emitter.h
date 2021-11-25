#ifndef CES_EVENT_EMITTER_H_
#define CES_EVENT_EMITTER_H_

#include "ces/utils/awaitable-data.h"

namespace ces {
struct EventEmitter {
  virtual utils::AwaitableData *emit() = 0;
  virtual void notify_arrival(utils::AwaitableData *) = 0;
  virtual void notify_departure(utils::AwaitableData *) = 0;
  virtual bool is_empty() = 0;
};
} // namespace ces

#endif