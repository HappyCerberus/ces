#ifndef OBSERVER_H_
#define OBSERVER_H_

#include "ces/utils/time-fwd.h"

#include <utility>

namespace ces::utils {

template <typename Data> struct ObserverWithDeadline {
  ObserverWithDeadline(Data &data, time_point deadline = NO_DEADLINE)
      : ptr_(&data), deadline_(deadline) {}
  ObserverWithDeadline(const ObserverWithDeadline &) = default;
  ObserverWithDeadline(ObserverWithDeadline &&o)
      : ptr_(std::exchange(o.ptr_, nullptr)),
        deadline_(std::exchange(o.deadline_, NO_DEADLINE)) {}
  ~ObserverWithDeadline() = default;
  ObserverWithDeadline &operator=(const ObserverWithDeadline &) = default;
  ObserverWithDeadline &operator=(ObserverWithDeadline &&o) {
    ptr_ = std::exchange(o.ptr_, nullptr);
    deadline_ = std::exchange(o.deadline_, NO_DEADLINE);
  }

  Data &operator*() { return *ptr_; }
  const Data &operator*() const { return *ptr_; }
  Data *operator->() { return ptr_; }
  const Data *operator->() const { return ptr_; }
  time_point deadline() const { return deadline_; }

private:
  Data *ptr_;
  time_point deadline_;
};

} // namespace ces::utils

#endif