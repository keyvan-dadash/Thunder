#pragma once

#include <atomic>
#include <cstdint>
#include <condition_variable>
#include <mutex>
#include <type_traits>

#include <thunder/synchronization/AbstractMutex.hpp>

namespace thunder {

  namespace synchronization {

    // smae as WordLock in Webkit(WTF)

    class SmartLock
    {
      public:

        explicit SmartLock() = default;

        ~SmartLock() = default;

        void lock();

        void unlock();

        bool isLocked();

      protected:

        static constexpr uintptr_t kClear = 0;
        static constexpr uintptr_t kIsLockedBit = 1;
        static constexpr uintptr_t kIsQueueLocked = 2;
        static constexpr uintptr_t kQueueHeadMask = 3;
        static constexpr uint8_t kSpinLockCnt = 40;

        void lockSlow();

        void unlockSlow();

        std::atomic_uintptr_t state;

      private:

        struct ThreadData
        {
          bool shouldPark { false };
          std::mutex parkingLock;
          std::condition_variable parkingCondition;
          ThreadData* next { nullptr };
          ThreadData* tail { nullptr };
        };
    };
  };
}

