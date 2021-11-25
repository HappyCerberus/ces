#ifndef CES_UTILS_EVENT_INFO_H_
#define CES_UTILS_EVENT_INFO_H_

namespace ces::utils {

enum class EventType : int { None, WakeUp, Timeout, Error, Hangup };

using EventID = int;

struct EventInfo {
  EventID id;
  EventType type;
  int err_data;
};

enum class EventCategory : int { NONE, EPOLL };

} // namespace ces::utils

#endif