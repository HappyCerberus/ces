#ifndef CES_UTILS_AWAITABLE_DATA_H_
#define CES_UTILS_AWAITABLE_DATA_H_

#include <coroutine>
#include <functional>
#include <iosfwd>

#include "ces/utils/event-info.h"
#include "ces/utils/time-fwd.h"

namespace ces::utils {

struct AwaitResult {
  EventType result_type;
  int err;                 ///< 0 for OK, otherwise errno
  std::string err_message; // Error message if any.
  constexpr operator bool() const { return result_type == EventType::WakeUp; }
  friend std::ostream &operator<<(std::ostream &s, const AwaitResult &res);
};

struct AwaitableData {
  AwaitableData(std::function<bool()> condition, time_point deadline)
      : continuation{std::noop_coroutine()}, condition(move(condition)),
        event_category(EventCategory::NONE), event_id(0),
        deadline(deadline), result{} {}

  AwaitableData(EventCategory cat, EventID id, time_point deadline)
      : continuation{std::noop_coroutine()}, condition{nullptr},
        event_category(cat), event_id(id), deadline(deadline), result{} {}

  std::coroutine_handle<> continuation;
  std::function<bool()> condition;
  EventCategory event_category;
  EventID event_id;
  time_point deadline;
  AwaitResult result;
};

} // namespace ces::utils

#endif