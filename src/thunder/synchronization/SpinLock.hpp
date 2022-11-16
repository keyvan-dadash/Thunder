/**
 * @file SpinLock.hpp
 */
#pragma once

#include <atomic>
#include <cstddef>
#include <new>
#include <thread>

namespace thunder {

namespace synchronization {

#ifdef __cpp_lib_hardware_interference_size
using std::hardware_constructive_interference_size;
using std::hardware_destructive_interference_size;
#else
// 64 bytes on x86-64 │ L1_CACHE_BYTES │ L1_CACHE_SHIFT │ __cacheline_aligned │
// ...
constexpr std::size_t hardware_constructive_interference_size =
    2 * sizeof(std::max_align_t);
constexpr std::size_t hardware_destructive_interference_size =
    2 * sizeof(std::max_align_t);
#endif

/**
 * \brief BaseSpinLockClass is a class which specify what attribute should
 * Spinlock have.
 * \tparam FitIntCacheline is a boolean which represent wheter add padding
 * to attribute in order fit in cacheline or not.
 */
template <bool FitIntCacheline>
class BaseSpinLockClass {
 protected:
  std::atomic_flag lock_ = ATOMIC_FLAG_INIT; /// underlaying lock.
};

template <>
class BaseSpinLockClass<true> {
 protected:
  std::atomic_flag lock_ = ATOMIC_FLAG_INIT;
  char padding[hardware_destructive_interference_size - sizeof(lock_)];
};

/**
 * \brief BaseSpinLock is a class which implementation the spinlock.
 * \tparam FitIntCacheline is a boolean which represent wheter add padding
 * to attribute in order fit in cacheline or not.
 */
template <bool FitIntCacheline>
class BaseSpinLock : public BaseSpinLockClass<FitIntCacheline> {
 public:
  BaseSpinLock() {}

  ~BaseSpinLock() {}

  /**
   * \brief lock the spinlock.
   */
  void lock() noexcept {
    while (this->lock_.test_and_set(std::memory_order_acquire)) {
      while (this->lock_.test(std::memory_order_relaxed))
        std::this_thread::yield();
    }
  }

  /**
   * \brief unlock the spinlock.
   */
  void unlock() noexcept { this->lock_.clear(std::memory_order_release); }

  /**
   * \brief try to lock the spinlock.
   * \return boolean which tell if locking operation was successful or not.
   */
  bool try_lock() noexcept {
    return !this->lock_.test_and_set(std::memory_order_acq_rel);
  }

  /**
   * \brief clear underlaying lock status.
   */
  void clear() noexcept { this->lock_.clear(std::memory_order_release); }
};

using SpinLock = BaseSpinLock<false>; /// Spinlock does not fit in the cacheline.
using CacheLineSpinLock = BaseSpinLock<true>; /// SpinLock that fit in the cacheline.
static_assert(sizeof(CacheLineSpinLock) ==
                  hardware_destructive_interference_size,
              "CacheLine Spinlock not in size cacheline");
}  // namespace synchronization
}  // namespace thunder
