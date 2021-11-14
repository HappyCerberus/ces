#ifndef OBSERVER_H_
#define OBSERVER_H_

namespace ces::utils {

template <typename Data> struct Observer {
  Observer(Data &data) : ptr_(&data) {}
  Observer(const Observer &) = default;
  Observer(Observer &&o) : ptr_(o.ptr_) {}
  ~Observer() = default;
  Observer &operator=(const Observer &) = default;
  Observer &operator=(Observer &&o) {
    ptr_ = o.ptr_;
    o.ptr_ = nullptr;
  }

  Data &operator*() { return *ptr_; }
  const Data &operator*() const { return *ptr_; }
  Data *operator->() { return ptr_; }
  const Data *operator->() const { return ptr_; }

private:
  Data *ptr_;
};

} // namespace ces::utils

#endif