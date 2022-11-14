#pragma once

namespace thunder {

namespace synchronization {

class AbstractMutex {
 public:
  AbstractMutex() = default;

  virtual void lock() = 0;

  virtual void unlock() = 0;

  virtual void isLocked() = 0;
};
};  // namespace synchronization
}  // namespace thunder
