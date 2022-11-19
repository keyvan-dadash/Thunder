/**
 * @file SmartLock.chpp
 * Contains SmartLock class which is new type of lock.
 */

#pragma once

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <thunder/synchronization/AbstractMutex.hpp>
#include <type_traits>

namespace thunder {

namespace synchronization {

// smae as WordLock in Webkit(WTF)

/**
 * \brief SmartLock is a some type of lock.
 *
 * SmartLock is a lock similar to WordLock in Webkit(WTF), and in some cases
 * it will outperform Mutex, but in some other cases it performance is worst than Mutex.
 * This lock should be use in area which the amount of time that critical section hold is small.
 */
class SmartLock {
 public:
  explicit SmartLock() = default;

  ~SmartLock() = default;

  /**
   * \brief Perform lock operation.
   */
  void lock();

  /**
   * \brief Perform unlock operation.
   */
  void unlock();

  /**
   * \brief Check if lock is held or not.
   */
  bool isLocked();

 protected:
  static constexpr uintptr_t kClear = 0;
  static constexpr uintptr_t kIsLockedBit = 1;
  static constexpr uintptr_t kIsQueueLocked = 2;
  static constexpr uintptr_t kQueueHeadMask = 3;
  static constexpr uint8_t kSpinLockCnt = 40;

  /**
   * Perform locking operation slow.
   */
  void lockSlow();

  /**
   * Peorf unlocking operation slow.
   */
  void unlockSlow();

  std::atomic_uintptr_t state; /// hold state of underlaying lock.

 private:

  /**
   * \brief ThreadData is a struct which hold state of each thread that willing to hold the lock.
   */
  struct ThreadData {
    bool shouldPark{false}; /// Should thread park in list and wait more.
    std::mutex parkingLock; /// mutex which condition variable wait on. 
    std::condition_variable parkingCondition; /// condition variable which all parking threads wait on.
    ThreadData* next{nullptr}; /// Next parking thread.
    ThreadData* tail{nullptr}; /// Pervious parking thread.
  };
};
};  // namespace synchronization
}  // namespace thunder
