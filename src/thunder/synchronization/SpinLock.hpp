#pragma once

#include <atomic>
#include <new>



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
    
    class SpinLock {

      public:

        SpinLock()
        {
            this->init();
        }

        ~SpinLock()
        {

        }


        void lock() noexcept
        {
            while(lock_.test_and_set(std::memory_order_acq_rel))
                                                                ;
        }

        void unlock() noexcept
        {
            lock_.clear(std::memory_order_release);
        }

        bool try_lock() noexcept
        {
            return !lock_.test_and_set(std::memory_order_acq_rel);
        }

        void clear() noexcept
        {
          lock_.clear();
        }

      private:

        void init()
        {
            lock_.clear();
        }


        std::atomic_flag lock_;


    };



    class CacheLineSpinLock {

      public:
        CacheLineSpinLock()
        {
            this->spinlock_.reset(new SpinLock());
        }

        void lock() noexcept { spinlock_.get()->lock(); }
        void unlock() noexcept { spinlock_.get()->unlock(); }
        bool try_lock() noexcept { return spinlock_.get()->try_lock(); }

      private:
        std::unique_ptr<SpinLock> spinlock_ = nullptr;

        char padding[hardware_destructive_interference_size - sizeof(spinlock_)];

    };

    static_assert(
        sizeof(CacheLineSpinLock) == hardware_destructive_interference_size,
         "CacheLine Spinlock not in size cacheline");




  }
}