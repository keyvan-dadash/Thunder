#pragma once

#include <atomic>
#include <cstdint>

#include <thunder/synchronization/AbstractMutex.hpp>

namespace thunder {

  namespace synchronization {

    // smae as WordLock in Webkit(WTF)
    class SmartMutex : public AbstractMutex
    {
      public:

        explicit SmartMutex() = default;

        ~SmartMutex() = default;

        void lock() override;

        void unlock() override;

        void isLocked() override;

      protected:

        static constexpr uintptr_t kIsLockedBit = 1;
        static constexpr uintptr_t kIsQueueLocked = 2;
        static constexpr uintptr_t kQueueHeadMask = 3;
        static constexpr uint8_t kSpinLockCnt = 40;

        void lockSlow();

        void unlockSlow();

        std::atomic_uintptr_t ptr;
    };
  };
}

