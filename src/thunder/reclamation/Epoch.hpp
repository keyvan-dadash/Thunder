
#pragma once

#include <atomic>
#include <iostream>
#include <cstddef>
#include <memory>
#include <thread>
#include <mutex>
#include <vector>
#include <new>
#include <functional>

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


#define EPOCH_ACTIVE_FLAG 0x80000000

namespace thunder {

  namespace reclamation {


    struct LocalEpoch
    {
      std::atomic_int64_t local_epoch = 0;
      int64_t hoho = 10;
    };

    template<typename T>
    struct LocalRecycle
    {
      std::vector<std::pair<std::remove_reference_t<T>*, int64_t>> retire_list_;
      ~LocalRecycle()
      {
        std::cout << "what?" << std::endl;
      }
    };



    template<typename T>
    class Epoch
    {
      public:
        Epoch(/* args */);

        ~Epoch();



        class epoch_guard
        {
          public:

            epoch_guard(LocalEpoch* local, std::function<int64_t()> f) : guard_local_epoch_(local), f(f)
            {

            }

            ~epoch_guard()
            {
              std::cout << "guard " << this->guard_local_epoch_->local_epoch.load(std::memory_order_relaxed) << std::endl;
              this->finish();
              std::cout << "after guard des" << this->guard_local_epoch_->local_epoch.load(std::memory_order_relaxed) << std::endl;
            }

            void finish()
            {
              this->guard_local_epoch_->local_epoch.store(0, std::memory_order_relaxed);
            }

          private:
            LocalEpoch* guard_local_epoch_;

            std::function<int64_t()> f;
        };


        void registerThisThread();

        void unregistedThisThread();

        epoch_guard enterCriticalPath();

        void retireObj(std::remove_reference_t<T>* t);

        void recycleLocalThread();

        void sync();

        int64_t currentGlobalEpoch();

      private:
        std::atomic_int64_t global_epoch_;

        pthread_key_t global_epoch_key_;

        pthread_key_t global_recycle_key_;

        std::vector<LocalEpoch*> registered_thread_epoch_;

        std::vector<LocalRecycle<T>*> registered_thread_recycle_;

        std::mutex mutex_;
    };
    
  }
}


#include "Epoch.cpp"