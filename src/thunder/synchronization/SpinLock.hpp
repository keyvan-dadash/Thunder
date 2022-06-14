#pragma once

#include <atomic>
#include <new>
#include <cstddef>
#include <thread>

namespace thunder {


  namespace synchronization {

    #ifdef __cpp_lib_hardware_interference_size
        using std::hardware_constructive_interference_size;
        using std::hardware_destructive_interference_size;
    #else
        // 64 bytes on x86-64 │ L1_CACHE_BYTES │ L1_CACHE_SHIFT │ __cacheline_aligned │ ...
        constexpr std::size_t hardware_constructive_interference_size
            = 2 * sizeof(std::max_align_t);
        constexpr std::size_t hardware_destructive_interference_size
            = 2 * sizeof(std::max_align_t);
    #endif

    template <bool FitIntCacheline>
    class BaseSpinLockClass
    {
      protected:
        std::atomic_flag lock_ = ATOMIC_FLAG_INIT;
    };

    template <>
    class BaseSpinLockClass<true>
    {
      protected:
        std::atomic_flag lock_ = ATOMIC_FLAG_INIT;
        char padding[hardware_destructive_interference_size - sizeof(lock_)];
    };


    template <bool FitIntCacheline>
    class BaseSpinLock : public BaseSpinLockClass<FitIntCacheline> {

      public:

        BaseSpinLock()
        {
        }

        ~BaseSpinLock()
        {

        }

        void lock() noexcept
        {
            while(this->lock_.test_and_set(std::memory_order_acquire)) {
              while (this->lock_.test(std::memory_order_relaxed))
                                  std::this_thread::yield();
            }
        }

        void unlock() noexcept
        {
            this->lock_.clear(std::memory_order_release);
        }

        bool try_lock() noexcept
        {
            return !this->lock_.test_and_set(std::memory_order_acq_rel);
        }

        void clear() noexcept
        {
          this->lock_.clear(std::memory_order_release);
        }
    };

    using SpinLock = BaseSpinLock<false>;
    using CacheLineSpinLock = BaseSpinLock<true>;
    static_assert(
      sizeof(CacheLineSpinLock) == hardware_destructive_interference_size,
      "CacheLine Spinlock not in size cacheline");
 }
}

